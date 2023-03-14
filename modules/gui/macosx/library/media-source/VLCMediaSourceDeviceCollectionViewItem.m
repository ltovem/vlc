/*****************************************************************************
 * VLCMediaSourceDeviceCollectionView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCMediaSourceDeviceCollectionViewItem.h"

#import "extensions/NSColor+VLCAdditions.h"

#import "views/VLCImageView.h"

NSString *VLCMediaSourceDeviceCellIdentifier = @"VLCMediaSourceDeviceCellIdentifier";

@interface VLCMediaSourceDeviceCollectionViewItem ()

@end

@implementation VLCMediaSourceDeviceCollectionViewItem

- (void)viewDidLoad
{
    [super viewDidLoad];

    _mediaImageView.cropsImagesToRoundedCorners = YES;
    _mediaImageView.contentGravity = VLCImageViewContentGravityCenter;
}

- (void)prepareForReuse
{
    [super prepareForReuse];
    self.mediaImageView.image = nil;
}

@end
