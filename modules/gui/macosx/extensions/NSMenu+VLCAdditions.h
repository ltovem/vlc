/*****************************************************************************
* NSMenu+ItemAddition.h: MacOS X interface module
*****************************************************************************
* Copyright (C) 2020 VLC authors and VideoLAN
*
* Author: Felix Paul Kühne <fkuehne at videolan dot org>
*
* SPDX-License-Identifier: GPL-2.0-or-later
*****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSMenu (VLCAdditions)

/* NSMenu's property itemArray becomes writeable in 10.14 and above only
 * so the following is provided as a convenience method to add multiple entries */
- (void)addMenuItemsFromArray:(NSArray <NSMenuItem *>*)array;

@end

NS_ASSUME_NONNULL_END
