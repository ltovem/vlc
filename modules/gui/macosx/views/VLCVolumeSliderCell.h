/*****************************************************************************
 * VLCVolumeSliderCell.m
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "views/VLCDefaultValueSliderCell.h"

@interface VLCVolumeSliderCell : VLCDefaultValueSliderCell

- (void)setSliderStyleLight;
- (void)setSliderStyleDark;

@end
