// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryVideoCollectionViewsStackViewController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryVideoCollectionViewsStackViewController : NSObject

@property (readwrite, assign, nonatomic) NSSize collectionViewItemSize;
@property (readwrite, assign, nonatomic) CGFloat collectionViewMinimumLineSpacing;
@property (readwrite, assign, nonatomic) CGFloat collectionViewMinimumInteritemSpacing;
@property (readwrite, assign, nonatomic) NSEdgeInsets collectionViewSectionInset;

@property (readwrite, assign, nonatomic) NSStackView *collectionsStackView;
@property (readwrite, assign, nonatomic) NSScrollView *collectionsStackViewScrollView;

- (void)reloadData;

@end

NS_ASSUME_NONNULL_END
