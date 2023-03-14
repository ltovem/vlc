/*****************************************************************************
 * NSColor+VLCAdditions.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "NSColor+VLCAdditions.h"

@implementation NSColor (VLCAdditions)

+ (instancetype)VLCAccentColor
{
    if (@available(macOS 10.14, *)) {
        return [NSColor controlAccentColor];
    }

    return [NSColor VLCOrangeElementColor];
}

+ (instancetype)VLCOrangeElementColor
{
    return [NSColor colorWithRed:1. green:.38 blue:.04 alpha:1.];
}

+ (instancetype)VLClibraryLightTitleColor
{
    return [NSColor colorWithRed:0.15 green:0.16 blue:0.17 alpha:1.];
}

+ (instancetype)VLClibraryDarkTitleColor
{
    return [NSColor colorWithRed:0.85 green:0.84 blue:0.83 alpha:1.];
}

+ (instancetype)VLClibrarySubtitleColor
{
    return [NSColor colorWithRed:0.52 green:0.57 blue:0.61 alpha:1.];
}

+ (instancetype)VLClibraryAnnotationColor
{
    return [NSColor whiteColor];
}

+ (instancetype)VLClibraryAnnotationBackgroundColor
{
    return [NSColor colorWithRed:0. green:0. blue:0. alpha:.4];
}

+ (instancetype)VLClibrarySeparatorLightColor
{
    return [NSColor colorWithRed:0.89 green:0.91 blue:0.93 alpha:1.];
}

+ (instancetype)VLClibrarySeparatorDarkColor
{
    return [NSColor colorWithRed:0.11 green:0.09 blue:0.07 alpha:1.];
}

+ (instancetype)VLClibraryProgressIndicatorBackgroundColor
{
    return [NSColor colorWithRed:37./255. green:41./255. blue:44./255. alpha:.8];
}

+ (instancetype)VLClibraryProgressIndicatorForegroundColor
{
    return [NSColor colorWithRed:246./255. green:127./255. blue:0. alpha:1.];
}

+ (instancetype)VLClibraryItemBorderLightColor
{
    return [NSColor colorWithRed:0.89 green:0.89 blue:0.91 alpha:1.];
}

+ (instancetype)VLClibraryItemBorderDarkColor
{
    return [NSColor colorWithRed:0.48 green:0.48 blue:0.48 alpha:1.];
}

+ (instancetype)VLClibraryCollectionViewItemBackgroundLightColor
{
    return [NSColor colorWithRed:0.23 green:0.23 blue:0.23 alpha:1.];
}

+ (instancetype)VLClibraryCollectionViewItemBackgroundDarkColor
{
    return [NSColor colorWithRed:0.23 green:0.23 blue:0.23 alpha:1.];
}

+ (instancetype)VLCSliderFillColor
{
    return [NSColor VLCAccentColor];
}


+ (instancetype)VLCSliderLightBackgroundColor
{
    return [NSColor colorWithCalibratedWhite:0.5 alpha:0.5];
}

+ (instancetype)VLCSliderDarkBackgroundColor
{
    return [NSColor colorWithCalibratedWhite:1 alpha:0.2];
}

@end
