/*****************************************************************************
 * macosx.m: MacOS X OpenGL provider
 *****************************************************************************
 * Copyright (C) 2001-2013 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <hartman at videolan dot org>
 *          Eric Petit <titer@m0k.org>
 *          Benjamin Pracht <bigben at videolan dot org>
 *          Damien Fouilleul <damienf at videolan dot org>
 *          Pierre d'Herbemont <pdherbemont at videolan dot org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *          David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *          Rémi Denis-Courmont
 *          Juho Vähä-Herttua <juhovh at iki dot fi>
 *          Laurent Aimar <fenrir _AT_ videolan _DOT_ org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#import <Cocoa/Cocoa.h>
#import <OpenGL/OpenGL.h>
#import <dlfcn.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_opengl.h>
#include <vlc_dialog.h>
#include <vlc_vout_window.H>

#include "opengl/gl_common.h"

/**
 * Forward declarations
 */

@interface VLCOpenGLVideoView : NSOpenGLView
{
    vlc_gl_t *_gl;
    BOOL _hasPendingReshape;
    BOOL has_first_frame;

    CGLContextObj locked_ctx;
}
+ (VLCOpenGLVideoView *)getNewView:(vlc_gl_t *)gl;
- (void)setFrameToBoundsOfView:(NSView *)parentView;
- (void)makeCurrent;
- (void)releaseCurrent;
- (void)swap;
@end

static void *OurGetProcAddress(vlc_gl_t *gl, const char *name)
{
    VLC_UNUSED(gl);

    return dlsym(RTLD_DEFAULT, name);
}

static void Close(vlc_gl_t *gl)
{
    VLCOpenGLVideoView *sys = (__bridge_transfer VLCOpenGLVideoView *)gl->sys;

    /* TODO: lock or dispatch for end of life */
#if 0
    @autoreleasepool {
        assert(sys->locked_ctx == NULL);
        free(gl->sys);

        VLCOpenGLVideoView *glView = sys->glView;
        NSView* viewContainer = nil; //sys->container;
        dispatch_async(dispatch_get_main_queue(), ^{
            if ([viewContainer respondsToSelector:@selector(removeVoutSubview:)]) {
                /* This will retain sys->glView */
                [viewContainer removeVoutSubview:sys->glView];
            }

            /* release on main thread as explained in Open() */
            [viewContainer release];
            [glView removeFromSuperview];
            [glView release];
        });
    }
#endif
}

/*****************************************************************************
 * Opengl callbacks
 *****************************************************************************/
static int OpenglLock (vlc_gl_t *gl)
{
    VLCOpenGLVideoView *view = (__bridge VLCOpenGLVideoView *)gl->sys;
    [view makeCurrent];
    return VLC_SUCCESS;
}

static void OpenglUnlock (vlc_gl_t *gl)
{
    VLCOpenGLVideoView *view = (__bridge VLCOpenGLVideoView *)gl->sys;
    [view releaseCurrent];
}

static void OpenglSwap (vlc_gl_t *gl)
{
    VLCOpenGLVideoView *view = (__bridge VLCOpenGLVideoView *)gl->sys;
    [view swap];
}

static void OpenglRender(vlc_gl_t *gl)
{
    VLCOpenGLVideoView *view = (__bridge VLCOpenGLVideoView *)gl->sys;
    dispatch_async(dispatch_get_main_queue(), ^{
        view.needsDisplay = YES;
        msg_Info(gl, "NEEDS DISPLAY SET");
    });
}

/*****************************************************************************
 * Our NSView object
 *****************************************************************************/
@implementation VLCOpenGLVideoView

#define VLCAssertMainThread() assert([[NSThread currentThread] isMainThread])

+ (VLCOpenGLVideoView *)getNewView:(vlc_gl_t *)gl
{
    return [[self alloc] init:gl];
}

/**
 * Gets called by the Open() method.
 */
- (id)init:(vlc_gl_t *)gl
{
    VLCAssertMainThread();

    _gl = gl;
    locked_ctx = nil;

    /* Warning - this may be called on non main thread */

    NSOpenGLPixelFormatAttribute attribs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAAccelerated,
        NSOpenGLPFANoRecovery,
        NSOpenGLPFAColorSize, 24,
        NSOpenGLPFAAlphaSize, 8,
        NSOpenGLPFADepthSize, 24,
        NSOpenGLPFAAllowOfflineRenderers,
        0
    };

    NSOpenGLPixelFormat *fmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];

    if (!fmt)
        return nil;

    self = [super initWithFrame:NSMakeRect(0,0,10,10) pixelFormat:fmt];
    //[fmt release];

    if (!self)
        return nil;

    /* enable HiDPI support */
    [self setWantsBestResolutionOpenGLSurface:YES];

    /* request our screen's HDR mode (introduced in OS X 10.11) */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpartial-availability"
    if ([self respondsToSelector:@selector(setWantsExtendedDynamicRangeOpenGLSurface:)]) {
        [self setWantsExtendedDynamicRangeOpenGLSurface:YES];
    }
#pragma clang diagnostic pop

    /* Swap buffers only during the vertical retrace of the monitor.
     http://developer.apple.com/documentation/GraphicsImaging/
     Conceptual/OpenGL/chap5/chapter_5_section_44.html */
    GLint params[] = { 1 };
    CGLSetParameter ([[self openGLContext] CGLContextObj], kCGLCPSwapInterval, params);

    [[NSNotificationCenter defaultCenter] addObserverForName:NSApplicationDidChangeScreenParametersNotification
                                                      object:[NSApplication sharedApplication]
                                                       queue:nil
                                                  usingBlock:^(NSNotification *notification) {
                                                      [self performSelectorOnMainThread:@selector(reshape)
                                                                             withObject:nil
                                                                          waitUntilDone:NO];
                                                  }];

    [self setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];
    return self;
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

/**
 * Gets called by the Open() method.
 */
- (void)setFrameToBoundsOfView:(NSView *)parentView
{
    [self setFrame:[parentView bounds]];
}

/**
 * Gets called when the vout will acquire the lock and flush.
 * (Non main thread).
 */
- (void)setVoutFlushing:(BOOL)flushing
{
    if (!flushing)
        return;
    @synchronized(self) {
        _hasPendingReshape = NO;
    }
}

/**
 * Can -drawRect skip rendering?.
 */
- (BOOL)canSkipRendering
{
    VLCAssertMainThread();

    @synchronized(self) {
        // TODO
        return false;
        //return !_hasPendingReshape && sys->has_first_frame;
    }
}

- (void)makeCurrent
{
    assert(locked_ctx == NULL);

    NSOpenGLContext *context = [self openGLContext];
    CGLContextObj cglcntx = [context CGLContextObj];

    CGLError err = CGLLockContext (cglcntx);
    if (kCGLNoError == err) {
        locked_ctx = cglcntx;
        [context makeCurrentContext];
        // TODO
    }
    // TODO
}

- (void)releaseCurrent
{
    CGLUnlockContext(locked_ctx);
    locked_ctx = NULL;
}

- (void)swap
{
    [[self openGLContext] flushBuffer];
}

/**
 * Local method that locks the gl context.
 */
- (BOOL)lockgl
{
    VLCAssertMainThread();
    NSOpenGLContext *context = [self openGLContext];
    CGLError err = CGLLockContext ([context CGLContextObj]);
    if (err == kCGLNoError)
        [context makeCurrentContext];
    return err == kCGLNoError;
}

/**
 * Local method that unlocks the gl context.
 */
- (void)unlockgl
{
    VLCAssertMainThread();
    CGLUnlockContext ([[self openGLContext] CGLContextObj]);
}

/**
 * Local method that force a rendering of a frame.
 * This will get called if Cocoa forces us to redraw (via -drawRect).
 */
- (void)render
{
    VLCAssertMainThread();

    BOOL hasFirstFrame;
    @synchronized(self) { // vd can be accessed from multiple threads
        hasFirstFrame = true;
    }

    // TODO
    if (!hasFirstFrame)
    {
        glClear (GL_COLOR_BUFFER_BIT);
        return;
    }

    vlc_gl_ReportRender(_gl);
}

/**
 * Method called by Cocoa when the view is resized.
 */
- (void)reshape
{
    VLCAssertMainThread();
    [super reshape];

    /* on HiDPI displays, the point bounds don't equal the actual pixel based bounds */
    NSRect bounds = [self convertRectToBacking:[self bounds]];
}

/**
 * Method called by Cocoa when the view is resized or the location has changed.
 * We just need to make sure we are locking here.
 */
- (void)update
{
    VLCAssertMainThread();
    BOOL success = [self lockgl];
    if (!success)
        return;

    [super update];

    [self unlockgl];
}

/**
 * Method called by Cocoa to force redraw.
 */
- (void)drawRect:(NSRect) rect
{
    VLCAssertMainThread();

    BOOL success = [self lockgl];
    if (!success)
        return;

    [self render];

    [self unlockgl];
}

- (void)renewGState
{
    // Comment take from Apple GLEssentials sample code:
    // https://developer.apple.com/library/content/samplecode/GLEssentials
    //
    // OpenGL rendering is not synchronous with other rendering on the OSX.
    // Therefore, call disableScreenUpdatesUntilFlush so the window server
    // doesn't render non-OpenGL content in the window asynchronously from
    // OpenGL content, which could cause flickering.  (non-OpenGL content
    // includes the title bar and drawing done by the app with other APIs)

    // In macOS 10.13 and later, window updates are automatically batched
    // together and this no longer needs to be called (effectively a no-op)
    [[self window] disableScreenUpdatesUntilFlush];

    [super renewGState];
}

- (BOOL)isOpaque
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return NO;
}

- (BOOL)mouseDownCanMoveWindow
{
    return NO;
}

@end

static int Open(vlc_gl_t *gl, unsigned width, unsigned height)
{
    if (gl->surface->type != VOUT_WINDOW_TYPE_NSOBJECT)
        return VLC_EGENERIC;

    @autoreleasepool {
        __block VLCOpenGLVideoView *sys = nil;

        if (!CGDisplayUsesOpenGLAcceleration (kCGDirectMainDisplay))
            msg_Err (gl, "no OpenGL hardware acceleration found. this can lead to slow output and unexpected results");

        /* Get the drawable object */
        NSView *container = (__bridge NSView*)gl->surface->handle.nsobject;
        assert(container != nil);

        /* This will be released in Close(), on
         * main thread, after we are done using it. */
        // TODO
        //container = [container retain];

        /* Get our main view*/
        dispatch_sync(dispatch_get_main_queue(), ^{
            sys = [VLCOpenGLVideoView getNewView:gl];
        });

        if (sys == nil) {
            msg_Err(gl, "Initialization of open gl view failed");
            goto error;
        }

        /* We don't wait, that means that we'll have to be careful about releasing
         * container.
         * That's why we'll release on main thread in Close(). */
        if ([container isKindOfClass:[NSView class]]) {
            NSView *parentView = container;
            dispatch_async(dispatch_get_main_queue(), ^{
                [parentView addSubview:sys];
                [sys setFrameToBoundsOfView:parentView];
            });
        } else {
            msg_Err(gl, "Invalid drawable-nsobject object. drawable-nsobject must either be an NSView or comply to the @protocol VLCVideoViewEmbedding.");
            goto error;
        }

        static const struct vlc_gl_operations gl_ops =
        {
            .make_current = OpenglLock,
            .release_current = OpenglUnlock,
            .swap = OpenglSwap,
            .get_proc_address = OurGetProcAddress,
            .render_next = OpenglRender,
        };
        gl->ops = &gl_ops;
        gl->sys = (__bridge_retained void*)sys;

        return VLC_SUCCESS;

    error:
        Close(gl);
        return VLC_EGENERIC;
    }
}

vlc_module_begin ()
    /* Will be loaded even without interface module. see voutgl.m */
    set_shortname ("Mac OS X")
    set_description (N_("Mac OS X OpenGL video output"))
    set_subcategory (SUBCAT_VIDEO_VOUT)
    set_callback(Open)
    set_capability("opengl", 300)
    add_shortcut ("macosx", "vout_macosx")
vlc_module_end ()
