// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCPlaylistModel.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Foundation/Foundation.h>
#import <vlc_playlist.h>

@class VLCPlaylistController;
@class VLCPlaylistItem;

NS_ASSUME_NONNULL_BEGIN

@interface VLCPlaylistModel : NSObject

@property (readwrite, assign) VLCPlaylistController *playlistController;
@property (readonly) NSUInteger numberOfPlaylistItems;

- (void)dropExistingData;
- (VLCPlaylistItem *)playlistItemAtIndex:(NSInteger)index;
- (void)addItems:(NSArray *)array;
- (void)addItems:(NSArray *)array atIndex:(size_t)index count:(size_t)count;
- (void)moveItemAtIndex:(size_t)index toTarget:(size_t)target;
- (void)removeItemsInRange:(NSRange)range;
- (void)updateItemAtIndex:(size_t)index;

@end

NS_ASSUME_NONNULL_END
