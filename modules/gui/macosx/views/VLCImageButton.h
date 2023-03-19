// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCImageButton.h
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Cameron Mozie <camsw0rld14@gmail.com>
 *          Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCImageButton : NSButton

@property (nonatomic) IBInspectable BOOL toggle;

@end
