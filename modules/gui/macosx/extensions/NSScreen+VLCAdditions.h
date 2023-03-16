// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * NSScreen+VLCAdditions.h: Category with some additions to NSScreen
 *****************************************************************************
 * Copyright (C) 2003-2014 VLC authors and VideoLAN
 *
 * Authors: Jon Lech Johansen <jon-vl@nanocrew.net>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface NSScreen (VLCAdditions)

+ (NSScreen *)screenWithDisplayID: (CGDirectDisplayID)displayID;
- (BOOL)hasMenuBar;
- (BOOL)hasDock;
- (BOOL)isScreen: (NSScreen*)screen;
- (CGDirectDisplayID)displayID;
- (void)blackoutOtherScreens;
+ (void)unblackoutScreens;

- (void)setFullscreenPresentationOptions;
- (void)setNonFullscreenPresentationOptions;

@end
