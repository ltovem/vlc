/*****************************************************************************
 * VLCScrollingClipView.h: NSClipView subclass that automatically scrolls
 *****************************************************************************
 * Copyright (C) 2015 VLC authors and VideoLAN
 *
 * Author: Marvin Scholz <epirat07@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

/**
 A Clip view subclass that offers automatic scrolling of its contents.
 */

#import <Cocoa/Cocoa.h>

@interface VLCScrollingClipView : NSClipView

/**
 \brief Start automatic scrolling

 \note  Does nothing if already scrolling
 */
- (void)startScrolling;

/**
 Stop automatic scrolling.

 \note  Does not reset the position */
- (void)stopScrolling;

/**
 Reset scrolling position to the top
 */
- (void)resetScrolling;

/**
 Outlet to the parent NSScrollView
 */
@property IBOutlet NSScrollView *parentScrollView;

@end
