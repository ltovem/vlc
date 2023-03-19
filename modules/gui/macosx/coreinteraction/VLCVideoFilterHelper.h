// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCVideoFilterHelper.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2006-2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne at videolan dot org>
 *****************************************************************************/

#import <Foundation/Foundation.h>
#import <vlc_common.h>

@interface VLCVideoFilterHelper : NSObject

+ (void)setVideoFilter: (const char *)psz_name on:(BOOL)b_on;
+ (void)setVideoFilterProperty: (const char *)psz_property forFilter: (const char *)psz_filter withValue: (vlc_value_t)value;

@end
