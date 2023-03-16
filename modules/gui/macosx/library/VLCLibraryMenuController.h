// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryMenuController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCInputItem;
@protocol VLCMediaLibraryItemProtocol;

@interface VLCLibraryMenuController : NSObject

@property (readonly) NSMenu *libraryMenu;
@property (readwrite, strong, nonatomic) id<VLCMediaLibraryItemProtocol> representedItem;
@property (readwrite, strong, nonatomic) VLCInputItem *representedInputItem;

- (void)popupMenuWithEvent:(NSEvent *)theEvent forView:(NSView *)theView;

@end

NS_ASSUME_NONNULL_END
