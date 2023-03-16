// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * NSGradient+VLCAdditions.m: Category that adds safer bezier path operations
 *****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot org>
 *****************************************************************************/

#import "NSGradient+VLCAdditions.h"

@implementation NSGradient (VLCAdditions)

- (void)vlc_safeDrawInBezierPath:(NSBezierPath *)path
                           angle:(CGFloat)angle
{
    if ([path isEmpty])
        return;
    [self drawInBezierPath:path angle:angle];
}

@end
