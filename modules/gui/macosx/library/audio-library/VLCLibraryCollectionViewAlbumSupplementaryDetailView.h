/*****************************************************************************
 * VLCLibraryCollectionViewAlbumSupplementaryDetailView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "library/VLCLibraryCollectionViewSupplementaryDetailView.h"

NS_ASSUME_NONNULL_BEGIN

@class VLCMediaLibraryAlbum;
@class VLCImageView;

extern NSString *const VLCLibraryCollectionViewAlbumSupplementaryDetailViewIdentifier;
extern NSCollectionViewSupplementaryElementKind const VLCLibraryCollectionViewAlbumSupplementaryDetailViewKind;

@interface VLCLibraryCollectionViewAlbumSupplementaryDetailView : VLCLibraryCollectionViewSupplementaryDetailView

@property (readwrite, retain, nonatomic) VLCMediaLibraryAlbum *representedAlbum;
@property (readwrite, weak) IBOutlet NSTextField *albumTitleTextField;
@property (readwrite, weak) IBOutlet NSTextField *albumDetailsTextField;
@property (readwrite, weak) IBOutlet NSTextField *albumYearAndDurationTextField;
@property (readwrite, weak) IBOutlet VLCImageView *albumArtworkImageView;
@property (readwrite, weak) IBOutlet NSTableView *albumTracksTableView;
@property (readwrite, weak) IBOutlet NSButton *playAlbumButton;

- (IBAction)playAction:(id)sender;
- (IBAction)enqueueAction:(id)sender;

@end

NS_ASSUME_NONNULL_END
