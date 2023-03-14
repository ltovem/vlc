/*****************************************************************************
 * VLCPlaylistModel.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCPlaylistModel.h"

#import <vlc_common.h>

#import "playlist/VLCPlaylistController.h"
#import "playlist/VLCPlaylistItem.h"

@interface VLCPlaylistModel ()
{
    NSMutableArray *_playlistArray;
}
@end

@implementation VLCPlaylistModel

- (instancetype)init
{
    self = [super init];
    if (self) {
        _playlistArray = [[NSMutableArray alloc] init];
    }
    return self;
}

- (NSUInteger)numberOfPlaylistItems
{
    return _playlistArray.count;
}

- (void)dropExistingData
{
    [_playlistArray removeAllObjects];
}

- (VLCPlaylistItem *)playlistItemAtIndex:(NSInteger)index
{
    if (index < 0 || index > _playlistArray.count) {
        return nil;
    }

    return _playlistArray[index];
}

- (void)addItems:(NSArray *)array
{
    [_playlistArray addObjectsFromArray:array];
}

- (void)addItems:(NSArray *)array atIndex:(size_t)index count:(size_t)count
{
    [_playlistArray insertObjects:array atIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(index, count)]];
}

- (void)moveItemAtIndex:(size_t)index toTarget:(size_t)target
{
    VLCPlaylistItem *item = [_playlistArray objectAtIndex:index];
    [_playlistArray removeObjectAtIndex:index];
    [_playlistArray insertObject:item atIndex:target];
}

- (void)removeItemsInRange:(NSRange)range
{
    [_playlistArray removeObjectsInRange:range];
}

- (void)updateItemAtIndex:(size_t)index
{
    VLCPlaylistItem *item = _playlistArray[index];
    [item updateRepresentation];
}

@end
