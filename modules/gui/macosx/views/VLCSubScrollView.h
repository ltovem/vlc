// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCSubScrollView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

// Use this scrollview when you are putting a scrollview within another scrollview.

@interface VLCSubScrollView : NSScrollView

@property (readwrite, assign) NSScrollView *parentScrollView;
@property (readwrite, assign) BOOL scrollParentY;
@property (readwrite, assign) BOOL scrollParentX;
@property (readwrite, assign) BOOL scrollSelf;

// Scroll views containing collection views can disobey hasVerticalScroller -> NO.
// This lets us forcefully override this behaviour
@property (readwrite, assign) BOOL forceHideVerticalScroller;
@property (readwrite, assign) BOOL forceHideHorizontalScroller;

@end

NS_ASSUME_NONNULL_END
