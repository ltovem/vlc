// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * VLCMinimalVoutWindow.m: macOS minimal vout window
 *****************************************************************************
 * Copyright (C) 2007-2017 VLC authors and VideoLAN
 *
 * Authors: Pierre d'Herbemont <pdherbemont # videolan.org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#import "VLCMinimalVoutWindow.h"
#import "misc.h"

#import <Cocoa/Cocoa.h>

@implementation VLCMinimalVoutWindow
- (id)initWithContentRect:(NSRect)contentRect
{
    if( self = [super initWithContentRect:contentRect
                                styleMask:NSBorderlessWindowMask
                                  backing:NSBackingStoreBuffered
                                    defer:NO])
    {
        initialFrame = contentRect;
        [self setBackgroundColor:[NSColor blackColor]];
        [self setHasShadow:YES];
        [self setMovableByWindowBackground:YES];
        [self center];
    }
    return self;
}

- (void)enterFullscreen
{
    NSScreen *screen = [self screen];

    initialFrame = [self frame];
    [self setFrame:[[self screen] frame] display:YES animate:YES];

    NSApplicationPresentationOptions presentationOpts = [NSApp presentationOptions];
    if ([screen hasMenuBar])
        presentationOpts |= NSApplicationPresentationAutoHideMenuBar;
    if ([screen hasMenuBar] || [screen hasDock])
        presentationOpts |= NSApplicationPresentationAutoHideDock;
    [NSApp setPresentationOptions:presentationOpts];
}

- (void)leaveFullscreen
{
    [NSApp setPresentationOptions: NSApplicationPresentationDefault];
    [self setFrame:initialFrame display:YES animate:YES];
}

@end
