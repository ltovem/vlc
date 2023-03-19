// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCMediaSourceProvider.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import "VLCMediaSourceProvider.h"

#import "VLCMediaSource.h"

#import "main/VLCMain.h"

#import <vlc_media_source.h>

static inline void getMediaSourcesForCategory(NSMutableArray <VLCMediaSource *> *const restrict __unsafe_unretained outputArray,
                                              libvlc_int_t *const restrict p_libvlcInstance,
                                              vlc_media_source_provider_t *const restrict p_sourceProvider,
                                              enum services_discovery_category_e const category)
{
    vlc_media_source_meta_list_t *const p_sourceMetaList = vlc_media_source_provider_List(p_sourceProvider,
                                                                                          category);
    const size_t count = vlc_media_source_meta_list_Count(p_sourceMetaList);

    for (size_t i = 0; i < count; ++i) {
        struct vlc_media_source_meta *const p_sourceMetaItem = vlc_media_source_meta_list_Get(p_sourceMetaList, i);

        vlc_media_source_t *const p_mediaSource = vlc_media_source_provider_GetMediaSource(p_sourceProvider, p_sourceMetaItem->name);
        if (p_mediaSource == NULL) {
            continue;
        }

        VLCMediaSource *const mediaSource = [[VLCMediaSource alloc] initWithMediaSource:p_mediaSource andLibVLCInstance:p_libvlcInstance forCategory:category];
        [outputArray addObject:mediaSource];
    }

    vlc_media_source_meta_list_Delete(p_sourceMetaList);
}

@implementation VLCMediaSourceProvider

+ (NSArray <VLCMediaSource *> *)listOfMediaSourcesForCategory:(enum services_discovery_category_e)category
{
    libvlc_int_t *p_libvlcInstance = vlc_object_instance(getIntf());
    vlc_media_source_provider_t *p_sourceProvider = vlc_media_source_provider_Get(p_libvlcInstance);

    if (p_sourceProvider == NULL) {
        return @[];
    }

    NSMutableArray *mutableArray = [[NSMutableArray alloc] initWithCapacity:32]; // A sane default

    getMediaSourcesForCategory(mutableArray, p_libvlcInstance, p_sourceProvider, category);

    return [mutableArray copy];
}

+ (NSArray<VLCMediaSource *> *)listOfLocalMediaSources;
{
    libvlc_int_t *p_libvlcInstance = vlc_object_instance(getIntf());
    vlc_media_source_provider_t *p_sourceProvider = vlc_media_source_provider_Get(p_libvlcInstance);

    NSMutableArray<VLCMediaSource *> *mutableArray = [[NSMutableArray alloc] initWithCapacity:32]; // A sane default
    [mutableArray addObject:[[VLCMediaSource alloc] initForLocalDevices:p_libvlcInstance]];

    if (p_sourceProvider != NULL) {
        // Currently, SD_CAT_MYCOMPUTER and SD_CAT_DEVICES return empty list.
        // They are left for future implementation.
        getMediaSourcesForCategory(mutableArray, p_libvlcInstance, p_sourceProvider, SD_CAT_MYCOMPUTER);
        getMediaSourcesForCategory(mutableArray, p_libvlcInstance, p_sourceProvider, SD_CAT_DEVICES);
        getMediaSourcesForCategory(mutableArray, p_libvlcInstance, p_sourceProvider, SD_CAT_LAN);
    }

    return [mutableArray copy];
}


@end




