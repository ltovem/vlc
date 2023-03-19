// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCMainVideoViewOverlayView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import "VLCMainVideoViewOverlayView.h"

@implementation VLCMainVideoViewOverlayView

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];

    // Drawing code here.
    NSColor *_darkestGradientColor = [NSColor colorWithWhite:0 alpha:0.4];

    NSGradient *gradient;

    if (_drawGradientForTopControls) {
        gradient = [[NSGradient alloc] initWithColorsAndLocations:_darkestGradientColor, 0.,
                    [NSColor clearColor], 0.5,
                    _darkestGradientColor, 1.,
                    nil];
    } else {
        gradient = [[NSGradient alloc] initWithColorsAndLocations:_darkestGradientColor, 0,
                    [NSColor clearColor], 1.,
                    nil];
    }

    // Draws bottom-up
    [gradient drawInRect:self.frame angle:90];
}

@end
