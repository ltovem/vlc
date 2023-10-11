/*****************************************************************************
 * VLCSampleBufferDisplay.m: video output display using
 * AVSampleBufferDisplayLayer on macOS
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Maxime Chapelet <umxprime at videolabs dot io>
 *
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_filter.h>
#include <vlc_plugin.h>
#include <vlc_modules.h>
#include <vlc_vout_display.h>
#include <vlc_atomic.h>
#include <vlc_interface.h>
#include <vlc_playlist.h>
#include <vlc_player.h>

# import <TargetConditionals.h>
# if TARGET_OS_OSX
#     import <Cocoa/Cocoa.h>
#     define VLCView NSView
# else
#     import <Foundation/Foundation.h>
#     import <UIKit/UIKit.h>
#     define VLCView UIView
# endif

#import <AVFoundation/AVFoundation.h>
#import <AVKit/AVKit.h>

#include "../../codec/vt_utils.h"

static vlc_decoder_device * CVPXHoldDecoderDevice(vlc_object_t *o, void *sys)
{
    VLC_UNUSED(o);
    vout_display_t *vd = sys;
    vlc_decoder_device *device =
        vlc_decoder_device_Create(VLC_OBJECT(vd), vd->cfg->window);
    static const struct vlc_decoder_device_operations ops =
    {
        NULL,
    };
    device->ops = &ops;
    device->type = VLC_DECODER_DEVICE_VIDEOTOOLBOX;
    return device;
}

static filter_t *
CreateCVPXConverter(vout_display_t *vd)
{
    filter_t *converter = vlc_object_create(vd, sizeof(filter_t));
    if (!converter)
        return NULL;

    static const struct filter_video_callbacks cbs =
    {
        .buffer_new = NULL,
        .hold_device = CVPXHoldDecoderDevice,
    };
    converter->owner.video = &cbs;
    converter->owner.sys = vd;

    es_format_InitFromVideo( &converter->fmt_in,  vd->fmt );
    es_format_InitFromVideo( &converter->fmt_out,  vd->fmt );

    converter->fmt_out.video.i_chroma =
    converter->fmt_out.i_codec = VLC_CODEC_CVPX_BGRA;

    converter->p_module = module_need(converter, "video converter", NULL, false);
    if (!converter->p_module)
    {
        vlc_object_delete(converter);
        return NULL;
    }
    assert( converter->ops != NULL );

    return converter;
}


static void DeleteCVPXConverter( filter_t * p_converter )
{
    if (!p_converter)
        return;

    if( p_converter->p_module )
    {
        filter_Close( p_converter );
        module_unneed( p_converter, p_converter->p_module );
    }

    es_format_Clean( &p_converter->fmt_in );
    es_format_Clean( &p_converter->fmt_out );

    vlc_object_delete(p_converter);
}

static intf_thread_t *p_interface_thread;
static vlc_player_t *p_player;

static vlc_player_t *GetPlayerFromInterface() {
    intf_thread_t *intf = p_interface_thread;
    if (!intf)
        return NULL;
    vlc_playlist_t *playlist = vlc_intf_GetMainPlaylist(intf);
    if (!playlist)
        return NULL;
    return vlc_playlist_GetPlayer(playlist);
}

static vlc_player_t *GetPlayer() {
    vlc_player_t *player = GetPlayerFromInterface();
    if (!player)
        player = p_player;
    return player;
}

typedef struct pipcontroller_t pipcontroller_t;

static void DeletePipController( pipcontroller_t * pipcontroller );

struct pipcontroller_operations {
    void (*set_display_layer)(pipcontroller_t *, void *);
};

struct pipcontroller_t
{
    struct vlc_object_t obj;

    /* Module properties */
    module_t *          p_module;
    void               *p_sys;

    const struct pipcontroller_operations *ops;
    vlc_player_listener_id *player_listener_id;

    bool did_seek;
};

/**
 * Protocol declaration that drawable-nsobject should follow
 */
@protocol VLCOpenGLVideoViewEmbedding <NSObject>
- (void)addVoutSubview:(VLCView *)view;
- (void)removeVoutSubview:(VLCView *)view;
@end

#pragma mark -
@class VLCSampleBufferSubpicture, VLCSampleBufferDisplay;

@interface VLCSampleBufferSubpictureRegion: NSObject
@property (nonatomic, weak) VLCSampleBufferSubpicture *subpicture;   
@property (nonatomic) CGRect backingFrame;
@property (nonatomic) CGImageRef image;
@end

@implementation VLCSampleBufferSubpictureRegion
- (void)dealloc {
    CGImageRelease(_image);
}
@end

#pragma mark -

@interface VLCSampleBufferSubpicture: NSObject
@property (nonatomic, weak) VLCSampleBufferDisplay *sys;
@property (nonatomic) NSArray<VLCSampleBufferSubpictureRegion *> *regions;
@property (nonatomic) int64_t order;
@end

@implementation VLCSampleBufferSubpicture

@end

#pragma mark -

@interface VLCSampleBufferSubpictureView: VLCView
- (void)drawSubpicture:(VLCSampleBufferSubpicture *)subpicture;
@end

@implementation VLCSampleBufferSubpictureView
{
    VLCSampleBufferSubpicture *_pendingSubpicture;
}

- (instancetype)init {
    self = [super init];
    if (!self)
        return nil;
#if TARGET_OS_OSX
    self.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    self.wantsLayer = YES;
#else
    self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
    self.backgroundColor = [UIColor clearColor];
#endif
    return self;
}

- (void)drawSubpicture:(VLCSampleBufferSubpicture *)subpicture {
    _pendingSubpicture = subpicture;
#if TARGET_OS_OSX
    [self setNeedsDisplay:YES];
#else
    [self setNeedsDisplay];
#endif
}

- (void)drawRect:(CGRect)dirtyRect {
    #if TARGET_OS_OSX
    NSGraphicsContext *graphicsCtx = [NSGraphicsContext currentContext];
    CGContextRef cgCtx = [graphicsCtx CGContext];
    #else
    CGContextRef cgCtx = UIGraphicsGetCurrentContext();
    #endif
    
    CGContextClearRect(cgCtx, self.bounds);
    
#if TARGET_OS_IPHONE
    CGContextSaveGState(cgCtx);
    CGAffineTransform translate = CGAffineTransformTranslate(CGAffineTransformIdentity, 0.0, self.frame.size.height);
    CGFloat scale = 1.0f / self.contentScaleFactor;
    CGAffineTransform transform = CGAffineTransformScale(translate, scale, -scale);
    CGContextConcatCTM(cgCtx, transform);
#endif
    VLCSampleBufferSubpictureRegion *region;
    for (region in _pendingSubpicture.regions) {
#if TARGET_OS_OSX
        CGRect regionFrame = [self convertRectFromBacking:region.backingFrame];
#else
        CGRect regionFrame = region.backingFrame;
#endif
        CGContextDrawImage(cgCtx, regionFrame, region.image);
    }
#if TARGET_OS_IPHONE
    CGContextRestoreGState(cgCtx);
#endif
}

@end

#pragma mark -

@interface VLCSampleBufferDisplayView: VLCView <CALayerDelegate>
@property (nonatomic, readonly) vout_display_t *vd;
- (instancetype)initWithVoutDisplay:(vout_display_t *)vd;
- (AVSampleBufferDisplayLayer *)displayLayer;
@end

@implementation VLCSampleBufferDisplayView

- (instancetype)initWithVoutDisplay:(vout_display_t *)vd {
    self = [super init];
    if (!self)
        return nil;
    _vd = vd;
#if TARGET_OS_OSX
    self.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    self.wantsLayer = YES;
#else
    self.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
#endif
    return self;
}

#if TARGET_OS_OSX
- (CALayer *)makeBackingLayer {
    AVSampleBufferDisplayLayer *layer;
    layer = [AVSampleBufferDisplayLayer new];
    layer.delegate = self;
    layer.videoGravity = AVLayerVideoGravityResizeAspect;
    [CATransaction lock];
    layer.needsDisplayOnBoundsChange = YES;
    layer.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
    layer.opaque = 1.0;
    layer.hidden = NO;
    [CATransaction unlock];
    return layer;
}
#else
+ (Class)layerClass {
    return [AVSampleBufferDisplayLayer class];
}
#endif

- (AVSampleBufferDisplayLayer *)displayLayer {
    return (AVSampleBufferDisplayLayer *)self.layer;
}

#if TARGET_OS_OSX
/* Layer delegate method that ensures the layer always get the
 * correct contentScale based on whether the view is on a HiDPI
 * display or not, and when it is moved between displays.
 */
- (BOOL)layer:(CALayer *)layer
shouldInheritContentsScale:(CGFloat)newScale
   fromWindow:(NSWindow *)window
{
    return YES;
}
#endif 

/*
 * General properties
 */

- (BOOL)isOpaque
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

@end

#pragma mark -

@interface VLCSampleBufferDisplay: NSObject
{
    @public
    vout_display_place_t place;
    filter_t *converter;
}
    @property (nonatomic, readonly) id<VLCOpenGLVideoViewEmbedding> container;
    @property (nonatomic, readonly) vout_display_t *vd;
    @property (nonatomic) VLCSampleBufferDisplayView *displayView;
    @property (nonatomic) AVSampleBufferDisplayLayer *displayLayer;
    @property (nonatomic) VLCSampleBufferSubpictureView *spuView;
    @property (nonatomic) VLCSampleBufferSubpicture *subpicture;

    @property (nonatomic, readonly) pipcontroller_t *pipcontroller;

    - (instancetype)init NS_UNAVAILABLE;
    + (instancetype)new NS_UNAVAILABLE;
    - (instancetype)initWithContainer:(id<VLCOpenGLVideoViewEmbedding>)container
                        pipController:(pipcontroller_t *)pipcontroller
                          voutDisplay:(vout_display_t *)vd;
@end

@implementation VLCSampleBufferDisplay

- (instancetype)initWithContainer:(id<VLCOpenGLVideoViewEmbedding>)container
                    pipController:(pipcontroller_t *)pipcontroller
                      voutDisplay:(vout_display_t *)vd
{
    self = [super init];
    if (!self)
        return nil;
    _container = container;
    _pipcontroller = pipcontroller;
    _vd = vd;
    
    return self;
}

- (void)prepareDisplay {
    @synchronized(_displayLayer) {
        if (_displayLayer)
            return;
    }

    VLCSampleBufferDisplay *sys = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        if (sys.displayView)
            return;
        VLCSampleBufferDisplayView *displayView = 
            [[VLCSampleBufferDisplayView alloc] initWithVoutDisplay:sys.vd];
        VLCSampleBufferSubpictureView *spuView = 
            [VLCSampleBufferSubpictureView new];
        id container = sys.container;
        //TODO: Is it still relevant ?
        if ([container respondsToSelector:@selector(addVoutSubview:)]) {
            [container addVoutSubview:displayView];
            [container addVoutSubview:spuView];
        } else if ([container isKindOfClass:[VLCView class]]) {
            VLCView *containerView = container;
            [containerView addSubview:displayView];
            [containerView addSubview:spuView];
            [displayView setFrame:containerView.bounds];
            [spuView setFrame:containerView.bounds];
        } else {
            displayView = nil;
            spuView = nil;
        }

        vout_display_PlacePicture(
            &sys->place, sys.vd->source, &sys.vd->cfg->display
        );

        sys.displayView = displayView;
        sys.spuView = spuView;
        @synchronized(sys.displayLayer) {
            sys.displayLayer = displayView.displayLayer;
            if ( sys.pipcontroller
                 && sys.pipcontroller->ops->set_display_layer ) {
                sys.pipcontroller->ops->set_display_layer(
                    sys.pipcontroller,
                    (__bridge void*)displayView.displayLayer
                );
            }
        }
    });
}

- (void)close {
    VLCSampleBufferDisplay *sys = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([sys.container respondsToSelector:@selector(removeVoutSubview:)]) {
            [sys.container removeVoutSubview:sys.displayView];
        }
        [sys.displayView removeFromSuperview];
        [sys.spuView removeFromSuperview];
    });
}

@end

#pragma mark -

@interface VLCPictureInPictureController: NSObject
    <AVPictureInPictureSampleBufferPlaybackDelegate, 
     AVPictureInPictureControllerDelegate>

@property (nonatomic, readonly) NSObject *avPipController;
@property (nonatomic, readonly) pipcontroller_t *pipcontroller;

- (instancetype)initWithPipController:(pipcontroller_t *)pipcontroller;
- (void)invalidatePlaybackState;

@end

@implementation VLCPictureInPictureController

- (instancetype)initWithPipController:(pipcontroller_t *)pipcontroller {
    self = [super init];
    if (!self)
        return nil;
    _pipcontroller = pipcontroller;
    
    return self;
}

- (void)invalidatePlaybackState {
    if (@available(macOS 12.0, iOS 15.0, tvos 15.0, *)) {
        AVPictureInPictureController *avPipController =
            (AVPictureInPictureController *)_avPipController;
        dispatch_async(dispatch_get_main_queue(),^{
            [avPipController invalidatePlaybackState];
        });
    }
}

- (void)prepare:(AVSampleBufferDisplayLayer *)layer {
    if (@available(macOS 12.0, iOS 15.0, tvos 15.0, *)) {
        if (![AVPictureInPictureController isPictureInPictureSupported]) {
            msg_Err(_pipcontroller, "Picture In Picture isn't supported");
            return;
        }
            
        assert(layer);
        AVPictureInPictureControllerContentSource *avPipSource;
        avPipSource = [
            [AVPictureInPictureControllerContentSource alloc]
                initWithSampleBufferDisplayLayer:layer
                                playbackDelegate:self
        ];
        AVPictureInPictureController *avPipController = [
            [AVPictureInPictureController alloc]
                initWithContentSource:avPipSource
        ];
        avPipController.delegate = self;
#if TARGET_OS_IOS
        // Not sure if it's mandatory, its usefulness isn't obvious and 
        // documentation doesn't particularily helps
        avPipController.canStartPictureInPictureAutomaticallyFromInline = YES;
#endif
        _avPipController = avPipController;
    } else {
        return;
    }


    [_avPipController addObserver:self
                       forKeyPath:@"isPictureInPicturePossible"
                          options:NSKeyValueObservingOptionInitial|NSKeyValueObservingOptionNew
                          context:NULL];
}

- (void)close {
    if (@available(macOS 12.0, iOS 15.0, tvos 15.0, *)) {
        AVPictureInPictureController *avPipController =
            (AVPictureInPictureController *)_avPipController;
        NSObject *observer = self;
        dispatch_async(dispatch_get_main_queue(),^{
            avPipController.contentSource = nil;
            [avPipController removeObserver:observer forKeyPath:@"isPictureInPicturePossible"];
        });
    }
    _avPipController = nil;
}

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object
                        change:(NSDictionary<NSKeyValueChangeKey,id> *)change
                       context:(void *)context {
    if (object==_avPipController) {
        if ([keyPath isEqualToString:@"isPictureInPicturePossible"]) {
            msg_Dbg(_pipcontroller, "isPictureInPicturePossible:%d", [change[NSKeyValueChangeNewKey] boolValue]);
        }
    } else {
        [super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
    }
}

#pragma mark - AVPictureInPictureSampleBufferPlaybackDelegate

- (void)pictureInPictureController:(nonnull AVPictureInPictureController *)pictureInPictureController
         didTransitionToRenderSize:(CMVideoDimensions)newRenderSize {
    
}

- (void)pictureInPictureController:(nonnull AVPictureInPictureController *)pictureInPictureController
                        setPlaying:(BOOL)playing {
    vlc_player_t *player = GetPlayer();
    if (!player) {
        msg_Dbg(_pipcontroller, "[pipcontroller] %s Player isn't available",
            __FUNCTION__);
        return;
    }
    vlc_player_Lock(player);
    if (!vlc_player_IsStarted(player)) {
        msg_Dbg(_pipcontroller, "[pipcontroller] %s Player isn't started",
            __FUNCTION__);
        vlc_player_Unlock(player);
        return;
    }
    if (playing) {
        if (vlc_player_IsPaused(player))
            vlc_player_Resume(player);
    } else {
        if (!vlc_player_IsPaused(player))
            vlc_player_Pause(player);
    }
    vlc_player_Unlock(player);
    msg_Dbg(_pipcontroller, "[pipcontroller] %s playing:%s",
            __FUNCTION__, playing?"true":"false");
}

- (void)pictureInPictureController:(nonnull AVPictureInPictureController *)pictureInPictureController
                    skipByInterval:(CMTime)skipInterval
                 completionHandler:(nonnull void (^)(void))completionHandler {
    vlc_player_t *player = GetPlayer();
    if (!player)
        return;
    Float64 time_sec = CMTimeGetSeconds(skipInterval);
    vlc_player_Lock(player);
    vlc_tick_t time = vlc_player_GetTime(player) + VLC_TICK_FROM_SEC(time_sec);
    vlc_player_SeekByTime(player, time, VLC_PLAYER_SEEK_FAST,
                          VLC_PLAYER_WHENCE_ABSOLUTE);
    if (vlc_player_IsPaused(player))
        vlc_player_Resume(player);
    vlc_player_Unlock(player);
    _pipcontroller->did_seek = true;
    completionHandler();
    msg_Dbg(_pipcontroller, "[pipcontroller] %s time_sec:%.02fs time:%lld", 
            __FUNCTION__, time_sec, time);
}

- (BOOL)pictureInPictureControllerIsPlaybackPaused:(nonnull AVPictureInPictureController *)pictureInPictureController {
    vlc_player_t *player = GetPlayer();
    if (!player) {
        msg_Dbg(_pipcontroller, "[pipcontroller] %s Player isn't available",
            __FUNCTION__);
        return YES;
    }
    vlc_player_Lock(player);
    if (!vlc_player_IsStarted(player)) {
        msg_Dbg(_pipcontroller, "[pipcontroller] %s Player isn't started",
            __FUNCTION__);
        vlc_player_Unlock(player);
        return YES;
    }
    BOOL isPaused = vlc_player_IsPaused(player);
    vlc_player_Unlock(player);
    msg_Dbg(_pipcontroller, "[pipcontroller] %s isPaused:%s",
            __FUNCTION__, isPaused?"true":"false");
    return isPaused;
}

- (CMTimeRange)pictureInPictureControllerTimeRangeForPlayback:(nonnull AVPictureInPictureController *)pictureInPictureController {
    //TODO: Handle media duration
    const CMTimeRange live = CMTimeRangeMake(kCMTimeNegativeInfinity, kCMTimePositiveInfinity);
    vlc_player_t *player = GetPlayer();
    if (!player)
        return live;
    vlc_player_Lock(player);
    vlc_tick_t length = vlc_player_GetLength(player);
    vlc_player_Unlock(player);
    if (length == VLC_TICK_INVALID)
        return live;
    vlc_player_Lock(player);
    vlc_tick_t time = vlc_player_GetTime(player);
    vlc_player_Unlock(player);
    if (time == VLC_TICK_INVALID)
        return live;

    CFTimeInterval ca_now = CACurrentMediaTime();
    CFTimeInterval time_sec = secf_from_vlc_tick(time);
    CFTimeInterval start = ca_now - time_sec;
    CFTimeInterval duration = secf_from_vlc_tick(length);
    msg_Dbg(_pipcontroller, "[pipcontroller] %s time_sec:%.02fs time:%lld", 
            __FUNCTION__, time_sec, time);
    return CMTimeRangeMake(CMTimeMakeWithSeconds(start, 1000000), CMTimeMakeWithSeconds(duration, 1000000));
}

- (BOOL)pictureInPictureControllerShouldProhibitBackgroundAudioPlayback:(AVPictureInPictureController *)pictureInPictureController {
    return NO;
}

#pragma mark - AVPictureInPictureControllerDelegate

- (void)pictureInPictureControllerWillStartPictureInPicture:(AVPictureInPictureController *)pictureInPictureController {
    /**
     * Invalidation seems mandatory as
     * pictureInPictureControllerTimeRangeForPlayback: isn't automatically
     * called each time PiP is activated
     */
    [self invalidatePlaybackState];
}

- (void)pictureInPictureControllerDidStartPictureInPicture:(AVPictureInPictureController *)pictureInPictureController {
}

- (void)pictureInPictureController:(AVPictureInPictureController *)pictureInPictureController
failedToStartPictureInPictureWithError:(NSError *)error {
}

- (void)pictureInPictureControllerWillStopPictureInPicture:(AVPictureInPictureController *)pictureInPictureController {
}

- (void)pictureInPictureControllerDidStopPictureInPicture:(AVPictureInPictureController *)pictureInPictureController {
}

- (void)pictureInPictureController:(AVPictureInPictureController *)pictureInPictureController
restoreUserInterfaceForPictureInPictureStopWithCompletionHandler:(void (^)(BOOL restored))completionHandler {
}

@end

#pragma mark -
#pragma mark Module functions

static void Close(vout_display_t *vd)
{
    VLCSampleBufferDisplay *sys;
    sys = (__bridge_transfer VLCSampleBufferDisplay*)vd->sys;

    DeleteCVPXConverter(sys->converter);
    DeletePipController(sys.pipcontroller);

    [sys close];
}

static void RenderPicture(vout_display_t *vd, picture_t *pic, vlc_tick_t date) {
    VLCSampleBufferDisplay *sys;
    sys = (__bridge VLCSampleBufferDisplay*)vd->sys;
    
    @synchronized(sys.displayLayer) {
        if (sys.displayLayer == nil)
            return;
    }

    picture_Hold(pic);

    picture_t *dst = pic;
    if (sys->converter) {
        dst = sys->converter->ops->filter_video(sys->converter, pic);
    }

    CVPixelBufferRef pixelBuffer = cvpxpic_get_ref(dst);
    CVPixelBufferRetain(pixelBuffer);
    picture_Release(dst);

    if (pixelBuffer == NULL) {
        msg_Err(vd, "No pixelBuffer ref attached to pic!");
        CVPixelBufferRelease(pixelBuffer);
        return;
    }

    id aspectRatio = @{
        (__bridge NSString*)kCVImageBufferPixelAspectRatioHorizontalSpacingKey:
            @(vd->source->i_sar_num),
        (__bridge NSString*)kCVImageBufferPixelAspectRatioVerticalSpacingKey:
            @(vd->source->i_sar_den)
    };

    CVBufferSetAttachment(
        pixelBuffer,
        kCVImageBufferPixelAspectRatioKey,
        (__bridge CFDictionaryRef)aspectRatio,
        kCVAttachmentMode_ShouldPropagate
    );

    CMSampleBufferRef sampleBuffer = NULL;
    CMVideoFormatDescriptionRef formatDesc = NULL;
    OSStatus err = CMVideoFormatDescriptionCreateForImageBuffer(kCFAllocatorDefault, pixelBuffer, &formatDesc);
    if (err != noErr) {
        msg_Err(vd, "Image buffer format desciption creation failed!");
        CVPixelBufferRelease(pixelBuffer);
        return;
    }

    vlc_tick_t now = vlc_tick_now();
    CFTimeInterval ca_now = CACurrentMediaTime();
    vlc_tick_t ca_now_ts = vlc_tick_from_sec(ca_now);
    vlc_tick_t diff = date - now;
    CFTimeInterval ca_date = secf_from_vlc_tick(ca_now_ts + diff);
    CMSampleTimingInfo sampleTimingInfo = {
        .decodeTimeStamp = kCMTimeInvalid,
        .duration = kCMTimeInvalid,
        .presentationTimeStamp = CMTimeMakeWithSeconds(ca_date, 1000000)
    };
    
    err = CMSampleBufferCreateReadyWithImageBuffer(kCFAllocatorDefault, pixelBuffer, formatDesc, &sampleTimingInfo, &sampleBuffer);
    CFRelease(formatDesc);
    CVPixelBufferRelease(pixelBuffer);
    if (err != noErr) {
        msg_Err(vd, "Image buffer creation failed!");
        return;
    }

    @synchronized(sys.displayLayer) {
        [sys.displayLayer enqueueSampleBuffer:sampleBuffer];
    }

    CFRelease(sampleBuffer);
}

static CGRect RegionBackingFrame(VLCSampleBufferDisplay* sys, 
                                 subpicture_t *subpicture,
                                 subpicture_region_t *r)
{
    const float scale_w = (float)(sys->place.width)  / subpicture->i_original_picture_width;
    const float scale_h = (float)(sys->place.height) / subpicture->i_original_picture_height;

    // Invert y coords for CoreGraphics
    const float y = subpicture->i_original_picture_height - r->fmt.i_visible_height - r->i_y;

    return CGRectMake(
        scale_w * r->i_x + sys->place.x, 
        scale_h * y + sys->place.y,
        scale_w * r->fmt.i_visible_width, 
        scale_h * r->fmt.i_visible_height
    );
}

static void UpdateSubpictureRegions(vout_display_t *vd, 
                                    subpicture_t *subpicture) 
{
    VLCSampleBufferDisplay *sys;
    sys = (__bridge VLCSampleBufferDisplay*)vd->sys;

    if (sys.subpicture == nil || subpicture == NULL)
        return;
    
    NSMutableArray *regions = [NSMutableArray new];
    CGColorSpaceRef space = CGColorSpaceCreateDeviceRGB();
    for (subpicture_region_t *r = subpicture->p_region; r; r = r->p_next) {
        CFIndex length = r->fmt.i_height * r->p_picture->p->i_pitch;
        const size_t pixels_offset =
                r->fmt.i_y_offset * r->p_picture->p->i_pitch +
                r->fmt.i_x_offset * r->p_picture->p->i_pixel_pitch;
        r->p_picture->p->i_visible_pitch = 
            r->fmt.i_visible_width * r->p_picture->p->i_pixel_pitch;
        
        CFDataRef data = CFDataCreate(
            NULL,
            r->p_picture->p->p_pixels + pixels_offset,
            length - pixels_offset);
        CGDataProviderRef provider = CGDataProviderCreateWithCFData(data);
        CGImageRef image = CGImageCreate(
            r->fmt.i_visible_width, r->fmt.i_visible_height, 
            8, 32, r->p_picture->p->i_pitch,  
            space, kCGImageByteOrderDefault | kCGImageAlphaFirst,
            provider, NULL, true, kCGRenderingIntentDefault
            );
        VLCSampleBufferSubpictureRegion *region;
        region = [VLCSampleBufferSubpictureRegion new];
        region.subpicture = sys.subpicture;
        region.image = image;

        region.backingFrame = RegionBackingFrame(sys, subpicture, r);
        [regions addObject:region];
        CGDataProviderRelease(provider);
        CFRelease(data);
    }
    CGColorSpaceRelease(space);

    sys.subpicture.regions = regions;
}

static bool IsSubpictureDrawNeeded(vout_display_t *vd, subpicture_t *subpicture)
{
    VLCSampleBufferDisplay *sys;
    sys = (__bridge VLCSampleBufferDisplay*)vd->sys;

    if (subpicture == NULL)
    {
        if (sys.subpicture == nil)
            return false;
        sys.subpicture = nil;
        /* Need to draw one last time in order to clear the current subpicture */
        return true;
    }

    size_t count = 0;
    for (subpicture_region_t *r = subpicture->p_region;
         r != NULL; r = r->p_next)
        count++;

    if (!sys.subpicture || subpicture->i_order != sys.subpicture.order)
    {
        /* Subpicture content is different */
        sys.subpicture = [VLCSampleBufferSubpicture new];
        sys.subpicture.sys = sys;
        sys.subpicture.order = subpicture->i_order;
        UpdateSubpictureRegions(vd, subpicture);
        return true;
    }

    bool draw = false;

    if (count == sys.subpicture.regions.count)
    {
        size_t i = 0;
        for (subpicture_region_t *r = subpicture->p_region;
             r != NULL; r = r->p_next)
        {
            VLCSampleBufferSubpictureRegion *region =
                sys.subpicture.regions[i++];

            CGRect newRegion = RegionBackingFrame(sys, subpicture, r);

            if ( !CGRectEqualToRect(region.backingFrame, newRegion) )
            {
                /* Subpicture regions are different */
                draw = true;
                break;
            }
        }
    }
    else
    {
        /* Subpicture region count is different */
        draw = true;
    }

    if (!draw)
        return false;

    /* Store the current subpicture regions in order to compare then later.
     */
    
    UpdateSubpictureRegions(vd, subpicture);
    return true;
}

static void RenderSubpicture(vout_display_t *vd, subpicture_t *spu) 
{
    if (!IsSubpictureDrawNeeded(vd, spu))
        return;
    
    VLCSampleBufferDisplay *sys;
    sys = (__bridge VLCSampleBufferDisplay*)vd->sys;
    
    dispatch_async(dispatch_get_main_queue(), ^{
        [sys.spuView drawSubpicture:sys.subpicture];
    });
}

static void PrepareDisplay (vout_display_t *vd) {
    VLCSampleBufferDisplay *sys;
    sys = (__bridge VLCSampleBufferDisplay*)vd->sys;

    [sys prepareDisplay];
}

static void Prepare (vout_display_t *vd, picture_t *pic, 
                           subpicture_t *subpicture, vlc_tick_t date)
{
    PrepareDisplay(vd);
    if (pic) {
        RenderPicture(vd, pic, date);
    }
    
    RenderSubpicture(vd, subpicture);
}

static void Display(vout_display_t *vd, picture_t *pic)
{
}

static int Control (vout_display_t *vd, int query)
{
    VLCSampleBufferDisplay *sys;
    sys = (__bridge VLCSampleBufferDisplay*)vd->sys;

    switch (query)
    {
        case VOUT_DISPLAY_CHANGE_DISPLAY_SIZE:
        case VOUT_DISPLAY_CHANGE_DISPLAY_FILLED:
        case VOUT_DISPLAY_CHANGE_ZOOM:
        case VOUT_DISPLAY_CHANGE_SOURCE_ASPECT:
        case VOUT_DISPLAY_CHANGE_SOURCE_CROP:
        {
            vout_display_PlacePicture(
                &sys->place, vd->source, &vd->cfg->display);
            break;
        }
        default:
            msg_Err (vd, "Unhandled request %d", query);
            return VLC_EGENERIC;
    }

    return VLC_SUCCESS;
}

static pipcontroller_t * CreatePipController( vout_display_t *vd )
{
    pipcontroller_t *pipcontroller = vlc_object_create(vd, sizeof(pipcontroller_t));
    pipcontroller->p_module = module_need(pipcontroller, "pipcontroller", NULL, false);
    if (!pipcontroller->p_module)
    {
        vlc_object_delete(pipcontroller);
        return NULL;
    }
    assert( pipcontroller->ops != NULL );
    return pipcontroller;
}

static void DeletePipController( pipcontroller_t * pipcontroller )
{
    if (pipcontroller == NULL)
        return;

    if( pipcontroller->p_module )
    {
        module_unneed( pipcontroller, pipcontroller->p_module );
    }

    vlc_object_delete(pipcontroller);
}

static int Open (vout_display_t *vd,
                 video_format_t *fmt, vlc_video_context *context)
{
    // Display isn't compatible with 360 content hence opening with this kind
    // of projection should fail if display use isn't forced
    if (!vd->obj.force && fmt->projection_mode != PROJECTION_MODE_RECTANGULAR) {
        return VLC_EGENERIC;
    }

    if (vd->cfg->window->type != VLC_WINDOW_TYPE_NSOBJECT)
        return VLC_EGENERIC;

    // Display will only work with CVPX video context
    filter_t *converter = NULL;
    if (!vlc_video_context_GetPrivate(context, VLC_VIDEO_CONTEXT_CVPX)) {
        converter = CreateCVPXConverter(vd);
        if (!converter)
            return VLC_EGENERIC;
    }

    @autoreleasepool {
        id container = (__bridge id)vd->cfg->window->handle.nsobject;
        if (!container) {
            msg_Err(vd, "No drawable-nsobject found!");
            DeleteCVPXConverter(converter);
            return VLC_EGENERIC;
        }

        VLCSampleBufferDisplay *sys = 
            [[VLCSampleBufferDisplay alloc] 
                initWithContainer:container
                    pipController:CreatePipController(vd)
                      voutDisplay:vd];

        if (sys == nil) {
            return VLC_ENOMEM;
        }

        sys->converter = converter;    

        vd->sys = (__bridge_retained void*)sys;

        static const struct vlc_display_operations ops = {
            Close, Prepare, Display, Control, NULL, NULL, NULL,
        };
        
        vd->ops = &ops;

        static const vlc_fourcc_t subfmts[] = {
            VLC_CODEC_ARGB,
            0
        };

        vd->info.subpicture_chromas = subfmts;
        
        return VLC_SUCCESS;
    }
}

static int OpenPlayer( vlc_object_t *p_this )
{
    struct vlc_player_probe_t *player_probe = (struct vlc_player_probe_t *)p_this;
    p_player = player_probe->player;
    return VLC_SUCCESS;
}

static int ClosePlayer( vlc_object_t *p_this )
{
    struct vlc_player_probe_t *player = (struct vlc_player_probe_t *)p_this;
    p_player = NULL;
    return VLC_SUCCESS;
}

static int OpenIntf( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t *)p_this;
    p_interface_thread = p_intf;
    return VLC_SUCCESS;
}

static int CloseIntf( vlc_object_t *p_this )
{
    intf_thread_t *p_intf = (intf_thread_t *)p_this;
    p_interface_thread = NULL;
    return VLC_SUCCESS;
}

static void SetDisplayLayer( pipcontroller_t *pipcontroller, void *layer) {
    VLCPictureInPictureController *sys = 
        (__bridge VLCPictureInPictureController*)pipcontroller->p_sys;
    
    AVSampleBufferDisplayLayer *displayLayer =
        (__bridge AVSampleBufferDisplayLayer *)layer;
    
    [sys prepare:displayLayer];
}

static void OnPlayerLengthChanged(vlc_player_t *player,
        vlc_tick_t new_length, void *data) {
    pipcontroller_t *pipcontroller = (pipcontroller_t *)data;
    VLCPictureInPictureController *sys = 
        (__bridge VLCPictureInPictureController*)pipcontroller->p_sys;
    [sys invalidatePlaybackState];
}

static void OnPlayerStateChanged(vlc_player_t *player,
        enum vlc_player_state new_state, void *data) {
    pipcontroller_t *pipcontroller = (pipcontroller_t *)data;
    VLCPictureInPictureController *sys = 
        (__bridge VLCPictureInPictureController*)pipcontroller->p_sys;
    [sys invalidatePlaybackState];
}

static void OnPlayerBufferingChanged(vlc_player_t *player,
        float new_buffering, void *data) {
    pipcontroller_t *pipcontroller = (pipcontroller_t *)data;
    VLCPictureInPictureController *sys = 
        (__bridge VLCPictureInPictureController*)pipcontroller->p_sys;
    [sys invalidatePlaybackState];
}

static void OnPlayerCurrentMediaChanged(vlc_player_t *player,
        input_item_t *new_media, void *data) {
    pipcontroller_t *pipcontroller = (pipcontroller_t *)data;
    VLCPictureInPictureController *sys = 
        (__bridge VLCPictureInPictureController*)pipcontroller->p_sys;
    [sys invalidatePlaybackState];
}

static void OnPlayerPositionChanged(vlc_player_t *player,
        vlc_tick_t new_time, double new_pos, void *data) {
    pipcontroller_t *pipcontroller = (pipcontroller_t *)data;
    VLCPictureInPictureController *sys = 
        (__bridge VLCPictureInPictureController*)pipcontroller->p_sys;
    if (pipcontroller->did_seek) {
        pipcontroller->did_seek = false;
        [sys invalidatePlaybackState];
    }
}

static const struct vlc_player_cbs player_callbacks = {
    .on_length_changed        = OnPlayerLengthChanged,
    .on_state_changed         = OnPlayerStateChanged,
    .on_buffering_changed     = OnPlayerBufferingChanged,
    .on_current_media_changed = OnPlayerCurrentMediaChanged,
    .on_position_changed      = OnPlayerPositionChanged,
};

static int OpenController( pipcontroller_t *pipcontroller )
{
    static const struct pipcontroller_operations ops = {
        SetDisplayLayer,
    };

    VLCPictureInPictureController *sys = 
        [[VLCPictureInPictureController alloc] 
            initWithPipController:pipcontroller];
    pipcontroller->p_sys = (__bridge_retained void*)sys;
    pipcontroller->ops = &ops;

    vlc_player_t *player = GetPlayer();
    if (player) {
        vlc_player_Lock(player);
        vlc_player_listener_id *listener = vlc_player_AddListener(player, &player_callbacks, pipcontroller);
        vlc_player_Unlock(player);
        pipcontroller->player_listener_id = listener;
    }

    return VLC_SUCCESS;
}

static int CloseController( pipcontroller_t *pipcontroller )
{
    VLCPictureInPictureController *sys = 
        (__bridge_transfer VLCPictureInPictureController*)pipcontroller->p_sys;
    
    [sys close];
    
    vlc_player_t *player = GetPlayer();
    if (player) {
        vlc_player_Lock(player);
        vlc_player_RemoveListener(player, pipcontroller->player_listener_id);
        vlc_player_Unlock(player);
    }
    
    return VLC_SUCCESS;
}

/*
 * Module descriptor
 */
vlc_module_begin()
    set_description(N_("CoreMedia sample buffers based video output display"))
    set_subcategory(SUBCAT_VIDEO_VOUT)
    set_callback_display(Open, 600)
    
    add_submodule()
        set_callbacks(OpenIntf, CloseIntf)
        set_capability("interface", 0)
        add_shortcut( "pipinterface" )
    
    add_submodule()
        set_callbacks(OpenPlayer, ClosePlayer)
        set_capability("playerprobe", 100)
    
    add_submodule()
        set_callbacks(OpenController, CloseController)
        set_capability("pipcontroller", 100)

vlc_module_end()
