/*****************************************************************************
* VLCLibraryInformationPanel.h: MacOS X interface module
*****************************************************************************
* Copyright (C) 2020 VLC authors and VideoLAN
*
* Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
*
* SPDX-License-Identifier: GPL-2.0-or-later
*****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@protocol VLCMediaLibraryItemProtocol;

@interface VLCLibraryInformationPanel : NSWindowController

@property (readwrite, weak) IBOutlet NSTextField *textField;
@property (readwrite, weak) IBOutlet NSImageView *imageView;

- (void)setRepresentedItem:(id<VLCMediaLibraryItemProtocol>)representedItem;

@end

NS_ASSUME_NONNULL_END
