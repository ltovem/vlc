// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibrarySongsTableViewSongPlayingTableCellView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@class VLCMediaLibraryMediaItem;

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibrarySongsTableViewSongPlayingTableCellView : NSTableCellView

@property (readwrite, weak, nonatomic) VLCMediaLibraryMediaItem *representedMediaItem;

@end

NS_ASSUME_NONNULL_END
