// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * NSFont+VLCAdditions.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSFont (VLCAdditions)

+ (instancetype)VLClibrarySectionHeaderFont;
+ (instancetype)VLClibraryHighlightCellTitleFont;
+ (instancetype)VLClibraryHighlightCellSubtitleFont;
+ (instancetype)VLClibraryHighlightCellHighlightLabelFont;
+ (instancetype)VLClibraryLargeCellTitleFont;
+ (instancetype)VLClibraryLargeCellSubtitleFont;
+ (instancetype)VLClibrarySmallCellTitleFont;
+ (instancetype)VLClibrarySmallCellSubtitleFont;
+ (instancetype)VLClibraryCellAnnotationFont;
+ (instancetype)VLClibraryButtonFont;
+ (instancetype)VLCplaylistLabelFont;
+ (instancetype)VLCplaylistSelectedItemLabelFont;
+ (instancetype)VLCsmallPlaylistLabelFont;
+ (instancetype)VLCsmallPlaylistSelectedItemLabelFont;
+ (instancetype)VLCLibrarySupplementaryDetailViewTitleFont;
+ (instancetype)VLCLibrarySupplementaryDetailViewSubtitleFont;

@end

NS_ASSUME_NONNULL_END
