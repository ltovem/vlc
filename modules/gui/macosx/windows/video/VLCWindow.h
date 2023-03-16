// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Windows.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2012-2018 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *          David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

/*****************************************************************************
 * VLCWindow
 *
 *  Missing extension to NSWindow
 *****************************************************************************/

@class VLCVoutView;

@interface VLCWindow : NSWindow <NSAnimationDelegate>

@property (readwrite) BOOL canBecomeKeyWindow;
@property (readwrite) BOOL canBecomeMainWindow;

@property (nonatomic, readwrite) BOOL hasActiveVideo;
@property (nonatomic, readwrite) BOOL fullscreen;
@property (readonly) BOOL isInNativeFullscreen;

- (void)closeAndAnimate:(BOOL)animate;
- (void)orderFront:(id)sender animate:(BOOL)animate;
- (void)orderOut:(id)sender animate:(BOOL)animate;

@end
