// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCVolumeSliderCell.m
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import "VLCVolumeSliderCell.h"

#import "extensions/NSGradient+VLCAdditions.h"
#import "extensions/NSColor+VLCAdditions.h"
#import "main/CompatibilityFixes.h"

@interface VLCVolumeSliderCell () {
    BOOL _isRTL;
    NSColor *_emptySliderBackgroundColor;
}
@end

@implementation VLCVolumeSliderCell

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self setSliderStyleLight];
        _isRTL = ([self userInterfaceLayoutDirection] == NSUserInterfaceLayoutDirectionRightToLeft);
    }
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        [self setSliderStyleLight];
        _isRTL = ([self userInterfaceLayoutDirection] == NSUserInterfaceLayoutDirectionRightToLeft);
    }
    return self;
}

- (void)setSliderStyleLight
{
    _emptySliderBackgroundColor = [NSColor VLCSliderLightBackgroundColor];
}

- (void)setSliderStyleDark
{
    _emptySliderBackgroundColor = [NSColor VLCSliderDarkBackgroundColor];
}

- (void)drawBarInside:(NSRect)rect flipped:(BOOL)flipped
{
    const CGFloat trackBorderRadius = 1;

    // Empty Track Drawing
    NSBezierPath* emptyTrackPath = [NSBezierPath bezierPathWithRoundedRect:rect
                                                                   xRadius:trackBorderRadius
                                                                   yRadius:trackBorderRadius];

    // Calculate filled track
    NSRect filledTrackRect = rect;
    NSRect knobRect = [self knobRectFlipped:NO];
    CGFloat sliderCenter = knobRect.origin.x  + (self.knobThickness / 2);

    if (_isRTL) {
        filledTrackRect.size.width = rect.origin.x + rect.size.width - sliderCenter;
        filledTrackRect.origin.x = sliderCenter;
    } else {
        filledTrackRect.size.width = sliderCenter;
    }

    NSBezierPath* filledTrackPath = [NSBezierPath bezierPathWithRoundedRect:filledTrackRect
                                                                    xRadius:trackBorderRadius
                                                                    yRadius:trackBorderRadius];
    NSColor *filledColor = [NSColor VLCSliderFillColor];

    [_emptySliderBackgroundColor setFill];
    [emptyTrackPath fill];
    [filledColor setFill];
    [filledTrackPath fill];
}

@end
