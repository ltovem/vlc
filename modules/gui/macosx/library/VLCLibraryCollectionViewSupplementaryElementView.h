// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewSupplementaryElementView.h: MacOS X interface module
 ******************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 ******************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

extern NSString *VLCLibrarySupplementaryElementViewIdentifier;

@interface VLCLibraryCollectionViewSupplementaryElementView : NSTextField

+ (CGSize)defaultHeaderSize;

@end

NS_ASSUME_NONNULL_END
