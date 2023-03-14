/*****************************************************************************
* VLCDetachedAudioWindow.h: macOS user interface
*****************************************************************************
* Copyright (C) 2019 VLC authors and VideoLAN
*
* Author: Felix Paul Kühne <fkuehne at videolan dot org>
*
* SPDX-License-Identifier: GPL-2.0-or-later
*****************************************************************************/

#import <Cocoa/Cocoa.h>
#import "windows/video/VLCWindow.h"

@class VLCControlsBarCommon;
@class VLCImageView;
@class VLCBottomBarView;

NS_ASSUME_NONNULL_BEGIN

@interface VLCDetachedAudioWindow : VLCWindow

@property (nonatomic, weak) IBOutlet NSView *wrapperView;
@property (nonatomic, weak) IBOutlet VLCImageView *imageView;
@property (nonatomic, weak) IBOutlet VLCControlsBarCommon *controlsBar;
@property (nonatomic, weak) IBOutlet VLCBottomBarView *bottomBarView;

@end

NS_ASSUME_NONNULL_END
