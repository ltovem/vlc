// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCInputNodePathControl.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCInputNodePathControlItem;

@interface VLCInputNodePathControl : NSPathControl

@property (readonly) NSMutableDictionary *inputNodePathControlItems;

- (void)appendInputNodePathControlItem:(VLCInputNodePathControlItem *)inputNodePathControlItem;
- (void)removeLastInputNodePathControlItem;
- (void)clearInputNodePathControlItems;
- (void)clearPathControlItemsAheadOf:(NSPathControlItem *)item;

@end

NS_ASSUME_NONNULL_END
