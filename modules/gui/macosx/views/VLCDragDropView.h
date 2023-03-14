/*****************************************************************************
 * VLCDragDropView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2003 - 2019 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <hartman # videolan dot org>
 *          Felix Paul Kühne <fkuehne # videolan dot org>
 *          David Fuhrmann <dfuhrmann # videolan dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

/*****************************************************************************
 * Disables default drag / drop behaviour of an NSImageView.
 * set it for all sub image views within an VLCDragDropView.
 *****************************************************************************/
@interface VLCDropDisabledImageView : NSImageView

@end

@protocol VLCDragDropTarget
@required
- (BOOL)handlePasteBoardFromDragSession:(NSPasteboard *)aPasteboard;
@end

@interface VLCDragDropView : NSView

@property (nonatomic, assign) id<VLCDragDropTarget> dropTarget;
@property (nonatomic, assign) BOOL drawBorder;

- (void)enablePlaylistItems;

@end

NS_ASSUME_NONNULL_END
