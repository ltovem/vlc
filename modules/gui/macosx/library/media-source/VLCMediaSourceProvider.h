// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCMediaSourceProvider.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Foundation/Foundation.h>

#import <vlc_services_discovery.h>

@class VLCMediaSource;

NS_ASSUME_NONNULL_BEGIN

@interface VLCMediaSourceProvider : NSObject

+ (NSArray <VLCMediaSource *> *)listOfMediaSourcesForCategory:(enum services_discovery_category_e)category;
+ (NSArray <VLCMediaSource *> *)listOfLocalMediaSources;

@end

NS_ASSUME_NONNULL_END
