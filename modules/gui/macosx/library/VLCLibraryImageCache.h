/*****************************************************************************
* VLCLibraryImageCache.h: MacOS X interface module
*****************************************************************************
* Copyright (C) 2020 VLC authors and VideoLAN
*
* Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
*
* SPDX-License-Identifier: GPL-2.0-or-later
*****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCInputItem;
@class VLCPlaylistItem;
@class VLCAbstractMediaLibraryItem;

@interface VLCLibraryImageCache : NSObject

+ (nullable NSImage *)thumbnailForLibraryItem:(VLCAbstractMediaLibraryItem*)libraryItem;
+ (nullable NSImage *)thumbnailForInputItem:(VLCInputItem*)inputItem;
+ (nullable NSImage *)thumbnailForPlaylistItem:(VLCPlaylistItem*)playlistItem;

@end

NS_ASSUME_NONNULL_END
