/*****************************************************************************
 * VLCLibraryVideoCollectionViewTableViewCellDataSource.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "library/VLCLibraryCollectionViewDataSource.h"

@class VLCLibraryVideoCollectionViewContainerView;
@class VLCLibraryVideoCollectionViewGroupDescriptor;

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryVideoCollectionViewContainerViewDataSource : NSObject <VLCLibraryCollectionViewDataSource>

@property (readwrite, assign) NSCollectionView *collectionView;
@property (readwrite, assign, nonatomic) VLCLibraryVideoCollectionViewGroupDescriptor *groupDescriptor;
@property (readwrite, assign) VLCLibraryVideoCollectionViewContainerView *parentCell;

- (void)setup;
- (void)reloadData;

@end

NS_ASSUME_NONNULL_END
