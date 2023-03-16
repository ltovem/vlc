// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCTrackingView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCTrackingView : NSView

@property (readwrite) BOOL animatesTransition;
@property (readwrite, assign, nullable) NSView *viewToHide;
@property (readwrite, assign, nullable) NSView *viewToShow;

@end

NS_ASSUME_NONNULL_END
