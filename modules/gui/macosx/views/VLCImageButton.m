// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCImageButton.m
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Cameron Mozie <camsw0rld14@gmail.com>
 *          Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import "VLCImageButton.h"

@implementation VLCImageButton

@synthesize toggle = _toggle;

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self =  [super initWithCoder:coder];
    if (self) {
        _toggle = NO;
    }
    return self;
}

- (void)awakeFromNib
{
    [(NSButtonCell*)[self cell] setHighlightsBy:NSPushInCellMask];
    [(NSButtonCell*)[self cell] setShowsStateBy:(_toggle) ? NSContentsCellMask : NSNoCellMask];
}

- (void)setToggle:(BOOL)toggle
{
    _toggle = toggle;
    [(NSButtonCell*)[self cell] setShowsStateBy:(_toggle) ? NSContentsCellMask : NSNoCellMask];
}

@end
