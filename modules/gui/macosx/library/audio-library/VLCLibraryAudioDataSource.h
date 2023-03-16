// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryAudioDataSource.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "library/VLCLibraryTableView.h"
#import "library/VLCLibraryCollectionViewDataSource.h"

NS_ASSUME_NONNULL_BEGIN

@class VLCLibraryModel;
@class VLCLibraryAudioGroupDataSource;
@class VLCMediaLibraryAlbum;

typedef NS_ENUM(NSUInteger, VLCAudioLibrarySegment) {
    VLCAudioLibraryArtistsSegment = 0,
    VLCAudioLibraryAlbumsSegment,
    VLCAudioLibrarySongsSegment,
    VLCAudioLibraryGenresSegment
};

@interface VLCLibraryAudioDataSource : NSObject <VLCLibraryTableViewDataSource, NSTableViewDelegate, VLCLibraryCollectionViewDataSource>

@property (readwrite, assign) VLCLibraryModel *libraryModel;
@property (readwrite, assign) VLCLibraryAudioGroupDataSource *audioGroupDataSource;
@property (readwrite, assign) NSTableView *collectionSelectionTableView;
@property (readwrite, assign) NSTableView *groupSelectionTableView;
@property (readwrite, assign) NSTableView *songsTableView;
@property (readwrite, assign) NSCollectionView *collectionView;
@property (readwrite, assign) NSTableView *gridModeListTableView;
@property (readwrite, assign) NSCollectionView *gridModeListSelectionCollectionView;

@property (nonatomic, readwrite, assign) VLCAudioLibrarySegment audioLibrarySegment;

- (void)setup;
- (void)setupCollectionView:(NSCollectionView *)collectionView;
- (void)reloadData;

@end

NS_ASSUME_NONNULL_END
