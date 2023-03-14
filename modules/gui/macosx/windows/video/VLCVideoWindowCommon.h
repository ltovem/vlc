/*****************************************************************************
 * Windows.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2012-2014 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *          David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "windows/video/VLCWindow.h"

extern NSString *VLCVideoWindowShouldShowFullscreenController;
extern NSString *VLCVideoWindowDidEnterFullscreen;
extern NSString *VLCWindowShouldShowController;
extern const CGFloat VLCVideoWindowCommonMinimalHeight;

@class VLCMainVideoViewController;
@class VLCPlayerController;

/*****************************************************************************
 * VLCVideoWindowCommon
 *
 *  Common code for main window, detached window and extra video window
 *****************************************************************************/

@interface VLCVideoWindowCommon : VLCWindow <NSWindowDelegate, NSAnimationDelegate>

@property (nonatomic, strong) IBOutlet VLCMainVideoViewController* videoViewController;
@property (readonly) BOOL inFullscreenTransition;
@property (readonly) BOOL windowShouldExitFullscreenWhenFinished;
@property (readwrite, assign) NSRect previousSavedFrame;
@property (readonly) VLCPlayerController *playerController;

- (void)setup;

- (void)setWindowLevel:(NSInteger)i_state;

/* fullscreen handling */
- (void)enterFullscreenWithAnimation:(BOOL)b_animation;
- (void)leaveFullscreenWithAnimation:(BOOL)b_animation;

- (void)windowWillEnterFullScreen:(NSNotification *)notification;
- (void)windowDidEnterFullScreen:(NSNotification *)notification;
- (void)windowWillExitFullScreen:(NSNotification *)notification;

@end
