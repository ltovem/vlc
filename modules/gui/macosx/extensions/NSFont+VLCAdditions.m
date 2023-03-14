/*****************************************************************************
 * NSFont+VLCAdditions.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "NSFont+VLCAdditions.h"

@implementation NSFont (VLCAdditions)

+ (instancetype)VLClibrarySectionHeaderFont
{
    return [NSFont systemFontOfSize:24. weight:NSFontWeightBold];
}

+ (instancetype)VLClibraryHighlightCellTitleFont
{
    return [NSFont systemFontOfSize:21. weight:NSFontWeightMedium];
}

+ (instancetype)VLClibraryHighlightCellSubtitleFont
{
    return [NSFont systemFontOfSize:13. weight:NSFontWeightSemibold];
}

+ (instancetype)VLClibraryHighlightCellHighlightLabelFont
{
    return [NSFont systemFontOfSize:11. weight:NSFontWeightBold];
}

+ (instancetype)VLClibraryLargeCellTitleFont
{
    return [NSFont systemFontOfSize:17. weight:NSFontWeightMedium];
}

+ (instancetype)VLClibraryLargeCellSubtitleFont
{
    return [NSFont systemFontOfSize:13. weight:NSFontWeightSemibold];
}

+ (instancetype)VLClibrarySmallCellTitleFont
{
    return [NSFont systemFontOfSize:13. weight:NSFontWeightMedium];
}

+ (instancetype)VLClibrarySmallCellSubtitleFont
{
    return [NSFont systemFontOfSize:10. weight:NSFontWeightSemibold];
}

+ (instancetype)VLClibraryCellAnnotationFont
{
    return [NSFont systemFontOfSize:15. weight:NSFontWeightBold];
}

+ (instancetype)VLClibraryButtonFont
{
    return [NSFont systemFontOfSize:15. weight:NSFontWeightBold];
}

+ (instancetype)VLCplaylistLabelFont
{
    return [NSFont systemFontOfSize:13. weight:NSFontWeightRegular];
}

+ (instancetype)VLCplaylistSelectedItemLabelFont
{
    return [NSFont systemFontOfSize:13. weight:NSFontWeightBold];
}

+ (instancetype)VLCsmallPlaylistLabelFont
{
    return [NSFont systemFontOfSize:10. weight:NSFontWeightRegular];
}

+ (instancetype)VLCsmallPlaylistSelectedItemLabelFont
{
    return [NSFont systemFontOfSize:10. weight:NSFontWeightBold];
}

+ (instancetype)VLCLibrarySupplementaryDetailViewTitleFont
{
    return [NSFont systemFontOfSize:20. weight:NSFontWeightSemibold];
}

+ (instancetype)VLCLibrarySupplementaryDetailViewSubtitleFont
{
    return [NSFont systemFontOfSize:18. weight:NSFontWeightMedium];
}

@end
