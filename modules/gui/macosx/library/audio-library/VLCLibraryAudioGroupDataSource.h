/*****************************************************************************
 * VLCLibraryAudioGroupDataSource.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "library/VLCLibraryCollectionViewDataSource.h"
#import "library/VLCLibraryTableView.h"

NS_ASSUME_NONNULL_BEGIN

@class VLCLibraryModel;
@class VLCLibraryAudioGroupDataSource;
@class VLCMediaLibraryAlbum;

@interface VLCLibraryAudioGroupDataSource : NSObject <VLCLibraryTableViewDataSource, NSTableViewDelegate, VLCLibraryCollectionViewDataSource>

@property (readwrite, retain, nullable) NSArray <VLCMediaLibraryAlbum *> *representedListOfAlbums;

@end

NS_ASSUME_NONNULL_END
