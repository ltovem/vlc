// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewSupplementaryDetailView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCSubScrollView;

// This class can be subclassed to create new supplementary detail views for library views.
// It provides a consistent background and properties. It is not meant to be used directly.

@interface VLCLibraryCollectionViewSupplementaryDetailView : NSView <NSCollectionViewElement>

@property (readwrite, assign) NSCollectionViewItem *selectedItem;
@property (readwrite, assign) NSScrollView *parentScrollView;
@property (readwrite, assign) VLCSubScrollView *internalScrollView;

@end

NS_ASSUME_NONNULL_END
