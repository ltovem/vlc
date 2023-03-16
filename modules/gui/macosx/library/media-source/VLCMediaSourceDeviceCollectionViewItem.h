// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCMediaSourceDeviceCollectionView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCImageView;

extern NSString *VLCMediaSourceDeviceCellIdentifier;

@interface VLCMediaSourceDeviceCollectionViewItem : NSCollectionViewItem

@property (readwrite, assign) IBOutlet NSTextField *titleTextField;
@property (readwrite, assign) IBOutlet VLCImageView *mediaImageView;

@end

NS_ASSUME_NONNULL_END
