// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCInputNodePathControlItem.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCInputNode;

@interface VLCInputNodePathControlItem : NSPathControlItem

@property (readonly) VLCInputNode *inputNode;

- (instancetype)initWithInputNode:(VLCInputNode *)inputNode;

@end

NS_ASSUME_NONNULL_END
