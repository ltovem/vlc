/*****************************************************************************
 * VLCPlaylistItem.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCPlaylistItem.h"

#import <vlc_input.h>
#import <vlc_url.h>

#import "extensions/NSString+Helpers.h"
#import "library/VLCInputItem.h"
#import "library/VLCLibraryDataTypes.h"

NSString *VLCPlaylistItemPasteboardType = @"org.videolan.vlc.playlistitemtype";

@implementation VLCPlaylistItem

- (instancetype)initWithPlaylistItem:(vlc_playlist_item_t *)p_item
{
    self = [super init];
    if (self) {
        _playlistItem = p_item;
        vlc_playlist_item_Hold(_playlistItem);
        [self updateRepresentation];
    }
    return self;
}

- (void)dealloc
{
    vlc_playlist_item_Release(_playlistItem);
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"item %p, title: %@ duration %lli", &_playlistItem, _title, _duration];
}

- (uint64_t)uniqueID
{
    return vlc_playlist_item_GetId(_playlistItem);
}

- (VLCInputItem *)inputItem
{
    if (!_playlistItem) {
        return nil;
    }
    input_item_t *p_input = vlc_playlist_item_GetMedia(_playlistItem);
    if (!p_input) {
        return nil;
    }
    VLCInputItem *inputItem = [[VLCInputItem alloc] initWithInputItem:p_input];
    return inputItem;
}

- (VLCMediaLibraryMediaItem *)mediaLibraryItem
{
    return [VLCMediaLibraryMediaItem mediaItemForURL:self.url];
}

- (void)updateRepresentation
{
    input_item_t *p_media = vlc_playlist_item_GetMedia(_playlistItem);
    vlc_mutex_lock(&p_media->lock);
    _title = toNSStr(p_media->psz_name);
    _duration = p_media->i_duration;

    if (p_media->p_meta) {
        _artistName = toNSStr(vlc_meta_Get(p_media->p_meta, vlc_meta_Artist));
        _albumName = toNSStr(vlc_meta_Get(p_media->p_meta, vlc_meta_Album));

        NSString *artworkURLString = toNSStr(vlc_meta_Get(p_media->p_meta, vlc_meta_ArtworkURL));
        if (artworkURLString.length > 1) {
            _artworkURL = [NSURL URLWithString:artworkURLString];
        }
    }
    vlc_mutex_unlock(&p_media->lock);
}

- (NSURL *)url
{
    if (_playlistItem) {
        return nil;
    }

    input_item_t *p_media = vlc_playlist_item_GetMedia(_playlistItem);
    if (!p_media) {
        return nil;
    }
    char *psz_url = input_item_GetURI(p_media);
    if (!psz_url)
        return nil;

    NSURL *url = [NSURL URLWithString:toNSStr(psz_url)];
    free(psz_url);
    return url;
}

- (NSString *)path
{
    if (!_playlistItem) {
        return nil;
    }
    input_item_t *p_media = vlc_playlist_item_GetMedia(_playlistItem);
    if (!p_media) {
        return nil;
    }
    char *psz_url = input_item_GetURI(p_media);
    if (!psz_url)
        return nil;

    char *psz_path = vlc_uri2path(psz_url);
    NSString *path = toNSStr(psz_path);
    free(psz_url);
    free(psz_path);
    return path;
}

@end
