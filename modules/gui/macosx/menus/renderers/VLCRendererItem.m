// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCRendererItem.m: Wrapper class for vlc_renderer_item_t
 *****************************************************************************
 * Copyright (C) 2016, 2019 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import "VLCRendererItem.h"

#import "extensions/NSString+Helpers.h"
#import "playlist/VLCPlayerController.h"

#include <vlc_common.h>
#include <vlc_renderer_discovery.h>

@implementation VLCRendererItem

- (instancetype)initWithRendererItem:(vlc_renderer_item_t*)item
{
    self = [super init];
    if (self) {
        if (!item)
            [NSException raise:NSInvalidArgumentException
                        format:@"item must not be nil"];
        _rendererItem = vlc_renderer_item_hold(item);
    }
    return self;
}

- (void)dealloc
{
    vlc_renderer_item_release(_rendererItem);
    _rendererItem = nil;
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@: name: %@, type: %@", [self className], self.name, self.type];
}

- (NSString *)name
{
    return toNSStr(vlc_renderer_item_name(_rendererItem));
}

- (NSString*)identifier
{
    return toNSStr(vlc_renderer_item_sout(_rendererItem));
}

- (NSString*)iconURI
{
    return toNSStr(vlc_renderer_item_icon_uri(_rendererItem));
}

- (NSString *)type
{
    return toNSStr(vlc_renderer_item_type(_rendererItem));
}

- (NSString *)userReadableType
{
    NSString *type = [self type];
    if ([type isEqualToString:@"stream_out_dlna"]) {
        return @"DLNA";
    } else if ([type isEqualToString:@"chromecast"]) {
        return @"Chromecast";
    }
    return type;
}

- (int)capabilityFlags
{
    return vlc_renderer_item_flags(_rendererItem);
}

- (void)setRendererForPlayerController:(VLCPlayerController *)playerController
{
    [playerController setRendererItem:_rendererItem];
}

- (BOOL)isEqual:(id)object
{
    if (![object isKindOfClass:[VLCRendererItem class]]) {
        return NO;
    }
    VLCRendererItem *other = object;
    return (other.rendererItem == self.rendererItem);
}

@end
