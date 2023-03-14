/*****************************************************************************
 * NSColor+VLCAdditions.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSColor (VLCAdditions)

+ (instancetype)VLCAccentColor;
+ (instancetype)VLCOrangeElementColor;
+ (instancetype)VLClibraryLightTitleColor;
+ (instancetype)VLClibraryDarkTitleColor;
+ (instancetype)VLClibrarySubtitleColor;
+ (instancetype)VLClibraryAnnotationColor;
+ (instancetype)VLClibraryAnnotationBackgroundColor;
+ (instancetype)VLClibrarySeparatorLightColor;
+ (instancetype)VLClibrarySeparatorDarkColor;
+ (instancetype)VLClibraryProgressIndicatorBackgroundColor;
+ (instancetype)VLClibraryProgressIndicatorForegroundColor;
+ (instancetype)VLClibraryItemBorderLightColor;
+ (instancetype)VLClibraryItemBorderDarkColor;
+ (instancetype)VLClibraryCollectionViewItemBackgroundLightColor;
+ (instancetype)VLClibraryCollectionViewItemBackgroundDarkColor;
+ (instancetype)VLCSliderFillColor;
+ (instancetype)VLCSliderLightBackgroundColor;
+ (instancetype)VLCSliderDarkBackgroundColor;

@end

NS_ASSUME_NONNULL_END
