// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewDelegate.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import "VLCLibraryCollectionViewDelegate.h"

#import "VLCLibraryCollectionViewDataSource.h"
#import "VLCLibraryCollectionViewFlowLayout.h"
#import "VLCLibraryCollectionViewItem.h"
#import "VLCLibraryDataTypes.h"

@implementation VLCLibraryCollectionViewDelegate

- (instancetype)init
{
    self = [super init];
    if (self) {
        _dynamicItemSizing = YES;
        _staticItemSize = [VLCLibraryCollectionViewItem defaultSize];
        _itemsAspectRatio = VLCLibraryCollectionViewItemAspectRatioDefaultItem;
    }
    return self;
}

- (void)collectionView:(NSCollectionView *)collectionView didSelectItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths
{
    NSIndexPath *indexPath = indexPaths.anyObject;
    if (!indexPath) {
        return;
    }

    VLCLibraryCollectionViewFlowLayout *collectionViewFlowLayout = (VLCLibraryCollectionViewFlowLayout*)collectionView.collectionViewLayout;
    if(collectionViewFlowLayout) {
        [collectionViewFlowLayout expandDetailSectionAtIndex:indexPath];
    }
}

- (void)collectionView:(NSCollectionView *)collectionView didDeselectItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths
{
    NSIndexPath *indexPath = indexPaths.anyObject;
    if (!indexPath) {
        return;
    }

    VLCLibraryCollectionViewFlowLayout *collectionViewFlowLayout = (VLCLibraryCollectionViewFlowLayout*)collectionView.collectionViewLayout;
    if (collectionViewFlowLayout) {
        [collectionViewFlowLayout collapseDetailSectionAtIndex:indexPath];
    }
}

- (NSSize)collectionView:(NSCollectionView *)collectionView
                  layout:(NSCollectionViewLayout *)collectionViewLayout
  sizeForItemAtIndexPath:(NSIndexPath *)indexPath
{
    if (!_dynamicItemSizing) {
        return _staticItemSize;
    }

    VLCLibraryCollectionViewFlowLayout *collectionViewFlowLayout = (VLCLibraryCollectionViewFlowLayout*)collectionViewLayout;
    if (collectionViewLayout) {
        VLCLibraryCollectionViewFlowLayout *collectionViewFlowLayout = (VLCLibraryCollectionViewFlowLayout*)collectionViewLayout;
        return [VLCLibraryUIUnits adjustedCollectionViewItemSizeForCollectionView:collectionView
                                                                       withLayout:collectionViewFlowLayout
                                                             withItemsAspectRatio:_itemsAspectRatio];
    }

    return NSZeroSize;
}

- (BOOL)collectionView:(NSCollectionView *)collectionView
canDragItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths
             withEvent:(NSEvent *)event
{
    return YES;
}

- (BOOL)collectionView:(NSCollectionView *)collectionView
writeItemsAtIndexPaths:(NSSet<NSIndexPath *> *)indexPaths
          toPasteboard:(NSPasteboard *)pasteboard
{
    if (![collectionView.dataSource conformsToProtocol:@protocol(VLCLibraryCollectionViewDataSource)]) {
        return NO;
    }

    NSObject<VLCLibraryCollectionViewDataSource> *vlcDataSource = (NSObject<VLCLibraryCollectionViewDataSource>*)collectionView.dataSource;

    NSUInteger numberOfIndexPaths = indexPaths.count;
    NSMutableArray *encodedLibraryItemsArray = [NSMutableArray arrayWithCapacity:numberOfIndexPaths];
    NSMutableArray *filePathsArray = [NSMutableArray arrayWithCapacity:numberOfIndexPaths];

    for (NSIndexPath *indexPath in indexPaths) {

        id<VLCMediaLibraryItemProtocol> libraryItem = [vlcDataSource libraryItemAtIndexPath:indexPath
                                                                          forCollectionView:collectionView];

        [libraryItem iterateMediaItemsWithBlock:^(VLCMediaLibraryMediaItem *mediaItem) {
            [encodedLibraryItemsArray addObject:mediaItem];

            VLCMediaLibraryFile *file = mediaItem.files.firstObject;
            if (file) {
                NSURL *url = [NSURL URLWithString:file.MRL];
                [filePathsArray addObject:url.path];
            }
        }];
    }

    NSData *data = [NSKeyedArchiver archivedDataWithRootObject:encodedLibraryItemsArray];
    [pasteboard declareTypes:@[VLCMediaLibraryMediaItemPasteboardType, NSFilenamesPboardType] owner:self];
    [pasteboard setPropertyList:filePathsArray forType:NSFilenamesPboardType];
    [pasteboard setData:data forType:VLCMediaLibraryMediaItemPasteboardType];

    return YES;
}

@end
