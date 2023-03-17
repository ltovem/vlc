// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * misc.h: custom code
 *****************************************************************************
 * Copyright (C) 2012 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne at videolan dot org>
 *          David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *          Pierre d'Herbemont <pdherbemont # videolan dot org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface NSScreen (VLCAdditions)
- (BOOL)hasMenuBar;
- (BOOL)hasDock;
- (CGDirectDisplayID)displayID;
@end
