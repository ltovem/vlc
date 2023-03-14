/*****************************************************************************
 * VLCVideoOutputProvider.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2012-2014 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *          David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import <vlc_window.h>

@class VLCControlsBarCommon;
@class VLCVideoWindowCommon;
@class VLCVoutView;

extern NSString *VLCWindowShouldUpdateLevel;
extern NSString *VLCWindowLevelKey;

@interface VLCVideoOutputProvider : NSObject

@property (readonly, nonatomic) NSInteger currentStatusWindowLevel;

- (VLCVoutView *)setupVoutForWindow:(vlc_window_t *)p_wnd withProposedVideoViewPosition:(NSRect)videoViewPosition;
- (void)removeVoutForDisplay:(NSValue *)o_key;
- (void)setNativeVideoSize:(NSSize)size forWindow:(vlc_window_t *)p_wnd;
- (void)setWindowLevel:(NSInteger)i_level forWindow:(vlc_window_t *)p_wnd;
- (void)setFullscreen:(int)i_full forWindow:(vlc_window_t *)p_wnd withAnimation:(BOOL)b_animation;

- (void)updateWindowLevelForHelperWindows:(NSInteger)i_level;

@end
