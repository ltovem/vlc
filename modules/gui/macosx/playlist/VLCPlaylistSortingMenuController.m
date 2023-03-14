/*****************************************************************************
 * VLCPlaylistSortingMenuController.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2002-2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCPlaylistSortingMenuController.h"

#import "extensions/NSString+Helpers.h"
#import "main/VLCMain.h"
#import "playlist/VLCPlaylistController.h"

@interface VLCPlaylistSortingMenuController () <NSMenuDelegate>
{
    VLCPlaylistController *_playlistController;
}
@end

@implementation VLCPlaylistSortingMenuController

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self createMenu];
        _playlistController = [[VLCMain sharedInstance] playlistController];
    }
    return self;
}

- (void)createMenu
{
    _playlistSortingMenu = [[NSMenu alloc] init];

    NSArray *titles = @[_NS("Title"),
                        _NS("Duration"),
                        _NS("Artist"),
                        _NS("Album"),
                        _NS("Album Artist"),
                        _NS("Genre"),
                        _NS("Date"),
                        _NS("Track Number"),
                        _NS("Disc Number"),
                        _NS("URL"),
                        _NS("Rating")];
    NSUInteger count = titles.count;
    for (NSUInteger x = 0; x < count; x++) {
        NSMenuItem *menuItem = [[NSMenuItem alloc] initWithTitle:titles[x] action:@selector(selectSortKey:) keyEquivalent:@""];
        menuItem.target = self;
        menuItem.tag = x;
        [_playlistSortingMenu addItem:menuItem];
    }

    [_playlistSortingMenu addItem:[NSMenuItem separatorItem]];

    titles = @[_NS("Ascending"),
               _NS("Descending")];
    count = titles.count;
    for (NSUInteger x = 0; x < count; x++) {
        NSMenuItem *menuItem = [[NSMenuItem alloc] initWithTitle:titles[x] action:@selector(selectSortOrder:) keyEquivalent:@""];
        menuItem.target = self;
        menuItem.tag = x + 100;
        [_playlistSortingMenu addItem:menuItem];
    }

    _playlistSortingMenu.delegate = self;
}

- (void)menuNeedsUpdate:(NSMenu *)menu
{
    if (_playlistController.unsorted) {
        return;
    }
    NSInteger count = _playlistSortingMenu.numberOfItems;
    for (NSInteger x = 0; x < count; x++) {
        NSMenuItem *menuItem = [_playlistSortingMenu itemAtIndex:x];
        menuItem.state = NSOffState;
    }

    NSMenuItem *menuItem = [_playlistSortingMenu itemWithTag:_playlistController.lastSortKey];
    menuItem.state = NSOnState;

    menuItem = [_playlistSortingMenu itemWithTag:_playlistController.lastSortOrder + 100];
    menuItem.state = NSOnState;
}

- (void)selectSortKey:(id)sender
{
    enum vlc_playlist_sort_key sortKey = (enum vlc_playlist_sort_key)[sender tag];
    enum vlc_playlist_sort_order sortOrder;
    if (_playlistController.unsorted) {
        /* we don't have an order and the user can only do a single selection - pick the most popular */
        sortOrder = VLC_PLAYLIST_SORT_ORDER_ASCENDING;
    } else {
        sortOrder = _playlistController.lastSortOrder;
    }

    [_playlistController sortByKey:sortKey andOrder:sortOrder];
}

- (void)selectSortOrder:(id)sender
{
    enum vlc_playlist_sort_key sortKey;
    enum vlc_playlist_sort_order sortOrder = (enum vlc_playlist_sort_order)([sender tag] - 100);
    if (_playlistController.unsorted) {
        /* we don't have a key and the user can only do a single selection - pick the most popular */
        sortKey = VLC_PLAYLIST_SORT_KEY_TITLE;
    } else {
        sortKey = _playlistController.lastSortKey;
    }

    [_playlistController sortByKey:sortKey andOrder:sortOrder];
}

@end
