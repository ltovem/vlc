/*****************************************************************************
 * VLCLibraryController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Foundation/Foundation.h>

#import <vlc_media_library.h>

@class VLCLibraryModel;
@class VLCMediaLibraryMediaItem;

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryController : NSObject

@property (readonly, nullable) VLCLibraryModel *libraryModel;

- (int)appendItemToPlaylist:(VLCMediaLibraryMediaItem *)mediaItem playImmediately:(BOOL)playImmediately;
- (int)appendItemsToPlaylist:(NSArray <VLCMediaLibraryMediaItem *> *)mediaItemArray playFirstItemImmediately:(BOOL)playFirstItemImmediately;

- (int)addFolderWithFileURL:(NSURL *)fileURL;
- (int)banFolderWithFileURL:(NSURL *)fileURL;
- (int)unbanFolderWithFileURL:(NSURL *)fileURL;
- (int)removeFolderWithFileURL:(NSURL *)fileURL;

- (int)clearHistory;

/**
 * Sort the entire library representation based on:
 * @param sortCriteria the criteria used for sorting
 * @param descending sort ascending or descending.
 */
- (void)sortByCriteria:(enum vlc_ml_sorting_criteria_t)sortCriteria andDescending:(bool)descending;

/**
 * @brief Filter the entire library representation based on:
 * @param filterString the string that will be used to filter items
 */
- (void)filterByString:(NSString*)filterString;

/**
 * Initially, the library is unsorted until the user decides to do so
 * Until then, the unsorted state is retained.
 */
@property (readonly) BOOL unsorted;

/**
 * The last key used for sorting
 */
@property (readonly) enum vlc_ml_sorting_criteria_t lastSortingCriteria;

/**
 * The last order used for sorting
 */
@property (readonly) bool descendingLibrarySorting;

@end

NS_ASSUME_NONNULL_END
