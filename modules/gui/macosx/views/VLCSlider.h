// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCSlider.h
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCSlider : NSSlider

@property (nonatomic, getter=getIndefinite,setter=setIndefinite:) BOOL indefinite;
@property (nonatomic, getter=getKnobHidden,setter=setKnobHidden:) BOOL isKnobHidden;

/* Indicates if the slider is scrollable with the mouse or trackpad scrollwheel. */
@property (readwrite) BOOL isScrollable;

@end
