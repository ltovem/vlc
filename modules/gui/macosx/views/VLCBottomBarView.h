// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCBottomBarView.h
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCBottomBarView : NSVisualEffectView

@property NSGradient *lightGradient;
@property NSGradient *darkGradient;

@property NSColor *lightStroke;
@property NSColor *darkStroke;

@property (readwrite) BOOL isDark;

@end
