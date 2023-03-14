/*****************************************************************************
 * VLCLibraryCollectionViewDelegate.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "VLCLibraryUIUnits.h"

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryCollectionViewDelegate : NSObject <NSCollectionViewDelegate, NSCollectionViewDelegateFlowLayout>

@property (readwrite, assign) BOOL dynamicItemSizing;
@property (readwrite, assign) NSSize staticItemSize;
@property (readwrite, assign) VLCLibraryCollectionViewItemAspectRatio itemsAspectRatio;

@end

NS_ASSUME_NONNULL_END
