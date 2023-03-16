// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCDefaultValueSlider.m: Custom NSSlider which allows a defaultValue
 *****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 -at- gmail -dot- com>
 *****************************************************************************/

#import "VLCDefaultValueSlider.h"

#import "views/VLCDefaultValueSliderCell.h"

@implementation VLCDefaultValueSlider

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        NSAssert([self.cell isKindOfClass:[VLCDefaultValueSliderCell class]],
                 @"VLCDefaultSlider cell is not VLCDefaultValueSliderCell");
        _isScrollable = YES;
    }
    return self;
}

+ (Class)cellClass
{
    return [VLCDefaultValueSliderCell class];
}

- (void)scrollWheel:(NSEvent *)event
{
    if (!_isScrollable)
        return [super scrollWheel:event];
    double increment;
    CGFloat deltaY = [event scrollingDeltaY];
    double range = [self maxValue] - [self minValue];

    // Scroll less for high precision, else it's too fast
    if (event.hasPreciseScrollingDeltas) {
        increment = (range * 0.002) * deltaY;
    } else {
        if (deltaY == 0.0)
            return;
        increment = (range * 0.01 * deltaY);
    }

    // If scrolling is inversed, increment in other direction
    if (!event.isDirectionInvertedFromDevice)
        increment = -increment;

    [self setDoubleValue:self.doubleValue - increment];
    [self sendAction:self.action to:self.target];
}

- (void)setDefaultValue:(double)value
{
    [(VLCDefaultValueSliderCell *)self.cell setDefaultValue:value];
}

- (double)defaultValue
{
    return [(VLCDefaultValueSliderCell *)self.cell defaultValue];
}

@end
