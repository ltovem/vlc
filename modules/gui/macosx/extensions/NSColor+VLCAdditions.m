/*****************************************************************************
 * NSColor+VLCAdditions.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#import "NSColor+VLCAdditions.h"

@implementation NSColor (VLCAdditions)

+ (instancetype)colorWithSRGBBytesRed:(uint8_t)r
                                green:(uint8_t)g
                                 blue:(uint8_t)b
{
    return [NSColor colorWithSRGBRed:(CGFloat)r/255.0
                               green:(CGFloat)g/255.0
                                blue:(CGFloat)b/255.0
                               alpha:1.0];
}

- (void)getBytesRed:(uint8_t *)red
              green:(uint8_t *)green
               blue:(uint8_t *)blue
{
    CGFloat normalizedRed;
    CGFloat normalizedGreen;
    CGFloat normalizedBlue;
    [self getRed:&normalizedRed
           green:&normalizedGreen
            blue:&normalizedBlue
           alpha:nil];

    if (red)
        *red = (uint8_t)(normalizedRed * 255);
    if (green)
        *green = (uint8_t)(normalizedGreen * 255);
    if (blue)
        *blue = (uint8_t)(normalizedBlue * 255);
}

+ (instancetype)VLCAccentColor
{
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
