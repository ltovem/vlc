/*****************************************************************************
 * VLCLibrarySongsTableViewSongPlayingTableCellView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCLibrarySongsTableViewSongPlayingTableCellView.h"

#import "library/VLCInputItem.h"
#import "library/VLCLibraryDataTypes.h"
#import "main/VLCMain.h"
#import "playlist/VLCPlayerController.h"
#import "playlist/VLCPlaylistController.h"

@implementation VLCLibrarySongsTableViewSongPlayingTableCellView

- (void)awakeFromNib
{
    NSNotificationCenter *notificationCenter = [NSNotificationCenter defaultCenter];
    [notificationCenter addObserver:self
                           selector:@selector(playStateOrItemChanged:)
                               name:VLCPlaylistCurrentItemChanged
                             object:nil];
    [notificationCenter addObserver:self
                           selector:@selector(playStateOrItemChanged:)
                               name:VLCPlayerStateChanged
                             object:nil];
}

- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void)prepareForReuse
{
    [super prepareForReuse];
    self.textField.stringValue = @"";
}

- (void)setRepresentedMediaItem:(VLCMediaLibraryMediaItem *)representedMediaItem
{
    _representedMediaItem = representedMediaItem;
    [self updatePlayState];
}

- (BOOL)isCurrentSong
{
    if (!_representedMediaItem ||
        !_representedMediaItem.inputItem ||
        !VLCMain.sharedInstance ||
        !VLCMain.sharedInstance.playlistController ||
        !VLCMain.sharedInstance.playlistController.currentlyPlayingInputItem) {

        return false;
    }

    return [_representedMediaItem.inputItem.MRL isEqualToString:VLCMain.sharedInstance.playlistController.currentlyPlayingInputItem.MRL];
}

- (void)updatePlayState
{
    if (!_representedMediaItem || ![self isCurrentSong]) {
        self.textField.stringValue = @"";
        return;
    }

    NSString *text = @"";

    switch(VLCMain.sharedInstance.playlistController.playerController.playerState) {
        case VLC_PLAYER_STATE_PAUSED:
            text = @"⏸︎";
            break;
        case VLC_PLAYER_STATE_PLAYING:
            text = @"▶";
        default:
            break;
    }

    self.textField.stringValue = text;
}

- (void)playStateOrItemChanged:(id)sender
{
    [self updatePlayState];
}

@end
