// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCVolumeSlider.m
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import "VLCVolumeSlider.h"

#import "extensions/NSView+VLCAdditions.h"
#import "views/VLCVolumeSliderCell.h"

@implementation VLCVolumeSlider

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];

    if (self) {
        NSAssert([self.cell isKindOfClass:[VLCVolumeSliderCell class]],
                 @"VLCVolumeSlider cell is not VLCVolumeSliderCell");

        if (@available(macOS 10.14, *)) {
            [self viewDidChangeEffectiveAppearance];
        } else {
            [(VLCVolumeSliderCell*)self.cell setSliderStyleLight];
        }
    }
    return self;
}

+ (Class)cellClass
{
    return [VLCVolumeSliderCell class];
}

// Workaround for 10.7
// http://stackoverflow.com/questions/3985816/custom-nsslidercell
- (void)setNeedsDisplayInRect:(NSRect)invalidRect {
    [super setNeedsDisplayInRect:[self bounds]];
}

- (void)viewDidChangeEffectiveAppearance
{
    if (self.shouldShowDarkAppearance) {
        [(VLCVolumeSliderCell*)self.cell setSliderStyleDark];
    } else {
        [(VLCVolumeSliderCell*)self.cell setSliderStyleLight];
    }
}

@end
