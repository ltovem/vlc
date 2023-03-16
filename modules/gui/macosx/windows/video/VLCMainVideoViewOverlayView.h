// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCMainVideoViewOverlayView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

/* Adds darkening gradients in the VLCMainVideoView to make controls more visible.
 * NOTE: Needs to occupy the entirety, and exactly, the area of the VLCMainVideoView */
@interface VLCMainVideoViewOverlayView : NSView

@property (readwrite, assign) BOOL drawGradientForTopControls;

@end

NS_ASSUME_NONNULL_END
