/*****************************************************************************
 * VLCLibraryUIUnits.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@class VLCLibraryCollectionViewFlowLayout;

typedef NS_ENUM(NSUInteger, VLCLibraryCollectionViewItemAspectRatio) {
    VLCLibraryCollectionViewItemAspectRatioDefaultItem = 0,
    VLCLibraryCollectionViewItemAspectRatioVideoItem,
};

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryUIUnits : NSObject

// Note that these values are not necessarily linked to the layout defined in the .xib files.
// If the spacing in the layout is changed you will want to change these values too.
+ (const CGFloat)largeSpacing;
+ (const CGFloat)mediumSpacing;
+ (const CGFloat)smallSpacing;

+ (const CGFloat)scrollBarSmallSideSize;

+ (const CGFloat)largeTableViewRowHeight;
+ (const CGFloat)mediumTableViewRowHeight;
+ (const CGFloat)smallTableViewRowHeight;

+ (const CGFloat)mediumDetailSupplementaryViewCollectionViewHeight;
+ (const CGFloat)largeDetailSupplementaryViewCollectionViewHeight;

+ (const CGFloat)dynamicCollectionViewItemMinimumWidth;
+ (const CGFloat)dynamicCollectionViewItemMaximumWidth;

+ (const CGFloat)collectionViewItemSpacing;
+ (const NSEdgeInsets)collectionViewSectionInsets;

+ (const NSSize)adjustedCollectionViewItemSizeForCollectionView:(NSCollectionView *)collectionView
                                                     withLayout:(VLCLibraryCollectionViewFlowLayout *)collectionViewLayout
                                           withItemsAspectRatio:(VLCLibraryCollectionViewItemAspectRatio)itemsAspectRatio;

+ (const NSEdgeInsets)libraryViewScrollViewContentInsets;
+ (const NSEdgeInsets)libraryViewScrollViewScrollerInsets;

+ (const CGFloat)controlsFadeAnimationDuration;

@end

NS_ASSUME_NONNULL_END
