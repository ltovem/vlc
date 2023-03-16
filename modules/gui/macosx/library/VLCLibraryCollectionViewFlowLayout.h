// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewFlowLayout.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryCollectionViewFlowLayout : NSCollectionViewFlowLayout

- (void)expandDetailSectionAtIndex:(NSIndexPath *)indexPath;
- (void)collapseDetailSectionAtIndex:(NSIndexPath *)indexPath;
- (void)resetLayout;

@end

NS_ASSUME_NONNULL_END
