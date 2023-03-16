// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCSliderCell.h
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCSliderCell : NSSliderCell

// Colors
@property NSColor *gradientColor;
@property NSColor *gradientColor2;
@property NSColor *trackStrokeColor;
@property NSColor *filledTrackColor;
@property NSColor *shadowColor;
@property NSColor *highlightBackground;

// Gradients
@property NSGradient *trackGradient;
@property NSGradient *highlightGradient;

@property NSInteger animationWidth;

@property (nonatomic, setter=setIndefinite:) BOOL indefinite;
@property (nonatomic, setter=setKnobHidden:) BOOL isKnobHidden;

- (void)setSliderStyleLight;
- (void)setSliderStyleDark;

@end
