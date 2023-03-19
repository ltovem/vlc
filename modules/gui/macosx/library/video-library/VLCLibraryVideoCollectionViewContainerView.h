// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryVideoCollectionViewContainerView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "library/video-library/VLCLibraryVideoGroupDescriptor.h"

@class VLCSubScrollView;
@class VLCLibraryCollectionViewDelegate;
@class VLCLibraryCollectionViewFlowLayout;
@class VLCLibraryVideoCollectionViewGroupDescriptor;
@class VLCLibraryVideoCollectionViewContainerViewDataSource;

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryVideoCollectionViewContainerView : NSView

@property (readonly) NSCollectionView *collectionView;
@property (readonly) VLCLibraryCollectionViewDelegate *collectionViewDelegate;
@property (readonly) VLCLibraryCollectionViewFlowLayout *collectionViewLayout;
@property (readonly) VLCSubScrollView *scrollView;
@property (readonly) VLCLibraryVideoCollectionViewContainerViewDataSource *dataSource;
@property (readonly) VLCLibraryVideoCollectionViewGroupDescriptor *groupDescriptor;
@property (readwrite, assign, nonatomic) VLCLibraryVideoGroup videoGroup;
@property (readwrite, assign) NSArray<NSLayoutConstraint *> *constraintsWithSuperview;

- (void)setVideoGroup:(VLCLibraryVideoGroup)group;

@end

NS_ASSUME_NONNULL_END
