/*****************************************************************************
 * VLCSlider.m
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCSlider.h"

#import "extensions/NSView+VLCAdditions.h"
#import "views/VLCSliderCell.h"

@implementation VLCSlider

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];

    if (self) {
        NSAssert([self.cell isKindOfClass:[VLCSliderCell class]],
                 @"VLCSlider cell is not VLCSliderCell");
        _isScrollable = YES;
        if (@available(macOS 10.14, *)) {
            [self viewDidChangeEffectiveAppearance];
        } else {
            [(VLCSliderCell*)self.cell setSliderStyleLight];
        }

    }
    return self;
}

+ (Class)cellClass
{
    return [VLCSliderCell class];
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

// Workaround for 10.7
// http://stackoverflow.com/questions/3985816/custom-nsslidercell
- (void)setNeedsDisplayInRect:(NSRect)invalidRect {
    [super setNeedsDisplayInRect:[self bounds]];
}

- (BOOL)getIndefinite
{
    return [(VLCSliderCell*)[self cell] indefinite];
}

- (void)setIndefinite:(BOOL)indefinite
{
    [(VLCSliderCell*)[self cell] setIndefinite:indefinite];
}

- (BOOL)getKnobHidden
{
    return [(VLCSliderCell*)[self cell] isKnobHidden];
}

- (void)setKnobHidden:(BOOL)isKnobHidden
{
    [(VLCSliderCell*)[self cell] setKnobHidden:isKnobHidden];
}

- (BOOL)isFlipped
{
    return NO;
}

- (void)viewDidChangeEffectiveAppearance
{
    if (self.shouldShowDarkAppearance) {
        [(VLCSliderCell*)self.cell setSliderStyleDark];
    } else {
        [(VLCSliderCell*)self.cell setSliderStyleLight];
    }
}

@end
