// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
* NSMenu+ItemAddition.m: MacOS X interface module
*****************************************************************************
* Copyright (C) 2020 VLC authors and VideoLAN
*
* Author: Felix Paul Kühne <fkuehne at videolan dot org>
*****************************************************************************/

#import "NSMenu+VLCAdditions.h"

@implementation NSMenu (VLCAdditions)

- (void)addMenuItemsFromArray:(NSArray <NSMenuItem *>*)array
{
    if (!array) {
        return;
    }

    for (NSMenuItem *item in array) {
        [self addItem:item];
    }
}

@end
