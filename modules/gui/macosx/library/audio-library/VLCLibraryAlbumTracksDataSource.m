// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryAlbumTracksDataSource.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import "VLCLibraryAlbumTracksDataSource.h"

#import "extensions/NSFont+VLCAdditions.h"
#import "extensions/NSString+Helpers.h"
#import "views/VLCImageView.h"
#import "views/VLCTrackingView.h"
#import "main/VLCMain.h"
#import "library/VLCLibraryController.h"
#import "library/VLCLibraryDataTypes.h"
#import "library/VLCLibraryTableCellView.h"
#import "library/VLCLibraryTableView.h"
#import "library/audio-library/VLCLibraryAlbumTracksDataSource.h"
#import "library/audio-library/VLCLibrarySongTableCellView.h"

const CGFloat VLCLibraryTracksRowHeight = 40.;

@interface VLCLibraryAlbumTracksDataSource ()
{
    NSArray *_tracks;
}
@end

@implementation VLCLibraryAlbumTracksDataSource

- (void)setRepresentedAlbum:(VLCMediaLibraryAlbum *)representedAlbum
{
    _representedAlbum = representedAlbum;
    _tracks = [_representedAlbum tracksAsMediaItems];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    if (_representedAlbum != nil) {
        return _representedAlbum.numberOfTracks;
    }

    return 0;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row
{
    VLCLibrarySongTableCellView *cellView = [tableView makeViewWithIdentifier:VLCAudioLibrarySongCellIdentifier owner:self];

    if (cellView == nil) {
        cellView = [VLCLibrarySongTableCellView fromNibWithOwner:self];
        cellView.identifier = VLCAudioLibrarySongCellIdentifier;
    }

    cellView.representedMediaItem = (VLCMediaLibraryMediaItem *)[self libraryItemAtRow:row
                                                                          forTableView:tableView];
    return cellView;
}

- (id<VLCMediaLibraryItemProtocol>)libraryItemAtRow:(NSInteger)row
                                       forTableView:(NSTableView *)tableView
{
    return _tracks[row];
}

@end
