/*****************************************************************************
 * VLCLibraryUIUnits.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCLibraryUIUnits.h"

#import "library/VLCLibraryCollectionViewFlowLayout.h"
#import "library/VLCLibraryCollectionViewItem.h"

@implementation VLCLibraryUIUnits

+ (const CGFloat)largeSpacing
{
    return 20;
}

+ (const CGFloat)mediumSpacing
{
    return 10;
}

+ (const CGFloat)smallSpacing
{
    return 5;
}

+ (const CGFloat)scrollBarSmallSideSize
{
    return 16;
}

+ (const CGFloat)largeTableViewRowHeight
{
    return 100;
}

+ (const CGFloat)mediumTableViewRowHeight
{
    return 50;
}

+ (const CGFloat)smallTableViewRowHeight
{
    return 25;
}

+ (const CGFloat)mediumDetailSupplementaryViewCollectionViewHeight
{
    return 300;
}

+ (const CGFloat)largeDetailSupplementaryViewCollectionViewHeight
{
    return 500;
}

+ (const CGFloat)dynamicCollectionViewItemMinimumWidth
{
    return 180;
}

+ (const CGFloat)dynamicCollectionViewItemMaximumWidth
{
    return 280;
}

+ (const CGFloat)collectionViewItemSpacing
{
    return [self largeSpacing];
}

+ (const NSEdgeInsets)collectionViewSectionInsets
{
    const CGFloat inset = [self largeSpacing];
    return NSEdgeInsetsMake(inset, inset, inset, inset);
}

+ (const NSSize)adjustedCollectionViewItemSizeForCollectionView:(NSCollectionView *)collectionView
                                                     withLayout:(VLCLibraryCollectionViewFlowLayout *)collectionViewLayout
                                           withItemsAspectRatio:(VLCLibraryCollectionViewItemAspectRatio)itemsAspectRatio
{
    static uint numItemsInRow = 5;

    NSSize itemSize = [self itemSizeForCollectionView:collectionView
                                           withLayout:collectionViewLayout
                                 withItemsAspectRatio:itemsAspectRatio
                               withNumberOfItemsInRow:numItemsInRow];

    while (itemSize.width > [VLCLibraryUIUnits dynamicCollectionViewItemMaximumWidth]) {
        ++numItemsInRow;
        itemSize = [self itemSizeForCollectionView:collectionView
                                        withLayout:collectionViewLayout
                              withItemsAspectRatio:itemsAspectRatio
                            withNumberOfItemsInRow:numItemsInRow];
    }
    while (itemSize.width < [VLCLibraryUIUnits dynamicCollectionViewItemMinimumWidth]) {
        --numItemsInRow;
        itemSize = [self itemSizeForCollectionView:collectionView
                                        withLayout:collectionViewLayout
                              withItemsAspectRatio:itemsAspectRatio
                            withNumberOfItemsInRow:numItemsInRow];
    }

    return itemSize;
}

+ (const NSSize)itemSizeForCollectionView:(NSCollectionView *)collectionView
                               withLayout:(VLCLibraryCollectionViewFlowLayout *)collectionViewLayout
                     withItemsAspectRatio:(VLCLibraryCollectionViewItemAspectRatio)itemsAspectRatio
                   withNumberOfItemsInRow:(uint)numItemsInRow
{
    NSParameterAssert(numItemsInRow > 0);
    NSParameterAssert(collectionView);
    NSParameterAssert(collectionViewLayout);

    const NSEdgeInsets sectionInsets = collectionViewLayout.sectionInset;
    const CGFloat interItemSpacing = collectionViewLayout.minimumInteritemSpacing;

    const CGFloat rowOfItemsWidth = collectionView.bounds.size.width -
                                    (sectionInsets.left +
                                     sectionInsets.right +
                                     (interItemSpacing * (numItemsInRow - 1)) +
                                     1);

    const CGFloat itemWidth = rowOfItemsWidth / numItemsInRow;
    const CGFloat itemHeight = itemsAspectRatio == VLCLibraryCollectionViewItemAspectRatioDefaultItem ?
        itemWidth + [VLCLibraryCollectionViewItem bottomTextViewsHeight] :
        (itemWidth * [VLCLibraryCollectionViewItem videoHeightAspectRatioMultiplier]) + [VLCLibraryCollectionViewItem bottomTextViewsHeight];

    return NSMakeSize(itemWidth, itemHeight);
}

+ (const NSEdgeInsets)libraryViewScrollViewContentInsets
{
    return NSEdgeInsetsMake([VLCLibraryUIUnits mediumSpacing],
                            [VLCLibraryUIUnits mediumSpacing],
                            [VLCLibraryUIUnits mediumSpacing],
                            [VLCLibraryUIUnits mediumSpacing]);
}

+ (const NSEdgeInsets)libraryViewScrollViewScrollerInsets
{
    const NSEdgeInsets contentInsets = [self libraryViewScrollViewContentInsets];
    return NSEdgeInsetsMake(-contentInsets.top,
                            -contentInsets.left,
                            -contentInsets.bottom,
                            -contentInsets.right);
}

+ (const CGFloat)controlsFadeAnimationDuration
{
    return 0.4f;
}

@end
