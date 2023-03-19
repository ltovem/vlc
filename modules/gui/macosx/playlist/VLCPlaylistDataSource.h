// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCPlaylistDataSource.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@class VLCPlaylistController;
@class VLCDragDropView;

NS_ASSUME_NONNULL_BEGIN

@interface VLCPlaylistDataSource : NSObject <NSTableViewDataSource, NSTableViewDelegate>

@property (readwrite, assign, nonatomic) VLCPlaylistController *playlistController;
@property (readwrite, assign) NSTableView *tableView;
@property (readwrite, assign) VLCDragDropView *dragDropView;
@property (readwrite, assign) NSTextField *counterTextField;

- (void)prepareForUse;
- (void)playlistUpdated;
- (void)scrollToCurrentPlaylistItem;

@end

NS_ASSUME_NONNULL_END
