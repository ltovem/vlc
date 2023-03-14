/*****************************************************************************
 * NSGradient+VLCAdditions.h: Category that adds safer bezier path operations
 *****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface NSGradient (VLCAdditions)

/* Safe alternative to drawInBezierPath:angle: which will throw an
 * exception when trying to draw into an empty NSBezierPath.
 */
- (void)vlc_safeDrawInBezierPath:(NSBezierPath *)path
                           angle:(CGFloat)angle;

@end
