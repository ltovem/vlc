// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewAudioGroupSupplementaryDetailView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import "VLCLibraryCollectionViewAudioGroupSupplementaryDetailView.h"

#import "library/VLCLibraryDataTypes.h"
#import "extensions/NSFont+VLCAdditions.h"

#import "library/audio-library/VLCLibraryAudioGroupDataSource.h"

NSString *const VLCLibraryCollectionViewAudioGroupSupplementaryDetailViewIdentifier = @"VLCLibraryCollectionViewAudioGroupSupplementaryDetailViewIdentifier";
NSCollectionViewSupplementaryElementKind const VLCLibraryCollectionViewAudioGroupSupplementaryDetailViewKind = @"VLCLibraryCollectionViewAudioGroupSupplementaryDetailViewIdentifier";

@interface VLCLibraryCollectionViewAudioGroupSupplementaryDetailView ()
{
    VLCLibraryAudioGroupDataSource *_audioGroupAlbumsDataSource;
}

@end

@implementation VLCLibraryCollectionViewAudioGroupSupplementaryDetailView

- (void)awakeFromNib
{
    _audioGroupAlbumsDataSource = [[VLCLibraryAudioGroupDataSource alloc] init];
    _audioGroupAlbumsTableView.dataSource = _audioGroupAlbumsDataSource;
    _audioGroupAlbumsTableView.delegate = _audioGroupAlbumsDataSource;

    _audioGroupNameTextField.font = [NSFont VLCLibrarySupplementaryDetailViewTitleFont];
}

- (void)setRepresentedAudioGroup:(id<VLCMediaLibraryAudioGroupProtocol>)representedAudioGroup
{
    _representedAudioGroup = representedAudioGroup;
    [self updateRepresentation];
}

- (void)updateRepresentation
{
    if (_representedAudioGroup == nil) {
        NSAssert(1, @"no media item assigned for collection view item", nil);
        return;
    }

    _audioGroupNameTextField.stringValue = _representedAudioGroup.displayString;
    _audioGroupAlbumsDataSource.representedListOfAlbums = _representedAudioGroup.albums;

    [_audioGroupAlbumsTableView reloadData];
}

@end
