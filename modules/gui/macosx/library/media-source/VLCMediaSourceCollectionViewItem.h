// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCMediaSourceCollectionViewItem.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

extern NSString *VLCMediaSourceCellIdentifier;

@class VLCImageView;
@class VLCInputItem;

@interface VLCMediaSourceCollectionViewItem : NSCollectionViewItem

@property (readwrite, assign) IBOutlet NSTextField *mediaTitleTextField;
@property (readwrite, assign) IBOutlet NSTextField *annotationTextField;
@property (readwrite, assign) IBOutlet VLCImageView *mediaImageView;
@property (readwrite, assign) IBOutlet NSButton *playInstantlyButton;
@property (readwrite, assign) IBOutlet NSButton *addToPlaylistButton;

@property (readwrite, strong, nonatomic) VLCInputItem *representedInputItem;

- (IBAction)playInstantly:(id)sender;
- (IBAction)addToPlaylist:(id)sender;

@end

NS_ASSUME_NONNULL_END
