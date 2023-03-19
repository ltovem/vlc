// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewMediaItemSupplementaryDetailView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>
#import "VLCLibraryCollectionViewSupplementaryDetailView.h"

NS_ASSUME_NONNULL_BEGIN

@class VLCMediaLibraryMediaItem;
@class VLCImageView;

extern NSString *const VLCLibraryCollectionViewMediaItemSupplementaryDetailViewIdentifier;
extern NSCollectionViewSupplementaryElementKind const VLCLibraryCollectionViewMediaItemSupplementaryDetailViewKind;

@interface VLCLibraryCollectionViewMediaItemSupplementaryDetailView : VLCLibraryCollectionViewSupplementaryDetailView

@property (readwrite, retain, nonatomic) VLCMediaLibraryMediaItem *representedMediaItem;
@property (readwrite, weak) IBOutlet NSTextField *mediaItemTitleTextField;
@property (readwrite, weak) IBOutlet NSTextField *mediaItemDetailTextField;
@property (readwrite, weak) IBOutlet NSTextField *mediaItemYearAndDurationTextField;
@property (readwrite, weak) IBOutlet NSTextField *mediaItemFileNameTextField;
@property (readwrite, weak) IBOutlet NSTextField *mediaItemPathTextField;
@property (readwrite, weak) IBOutlet VLCImageView *mediaItemArtworkImageView;
@property (readwrite, weak) IBOutlet NSButton *playMediaItemButton;

- (IBAction)playAction:(id)sender;
- (IBAction)enqueueAction:(id)sender;

@end

NS_ASSUME_NONNULL_END
