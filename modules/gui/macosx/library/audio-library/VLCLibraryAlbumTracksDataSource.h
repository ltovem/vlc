// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryAlbumTracksDataSource.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "library/VLCLibraryTableView.h"

NS_ASSUME_NONNULL_BEGIN

@class VLCMediaLibraryAlbum;

@interface VLCLibraryAlbumTracksDataSource : NSObject <VLCLibraryTableViewDataSource, NSTableViewDelegate>

extern const CGFloat VLCLibraryTracksRowHeight;

@property (readwrite, retain, nonatomic, nullable) VLCMediaLibraryAlbum *representedAlbum;

@end

NS_ASSUME_NONNULL_END
