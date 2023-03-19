// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import "VLCBasicView.h"

@implementation VLCBasicView

- (void)awakeFromNib
{
    self.wantsLayer = YES;
}

- (BOOL)wantsUpdateLayer {
   return YES;
}

- (void)updateLayer {
   self.layer.backgroundColor = [NSColor controlBackgroundColor].CGColor;
}

@end
