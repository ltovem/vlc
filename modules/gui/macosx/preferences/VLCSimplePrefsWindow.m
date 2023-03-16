// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCSimplePrefsWindow.m: Simple Preferences Window subclass for Mac OS X
 *****************************************************************************
 * Copyright (C) 2008-2014 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne at videolan dot org>
 *****************************************************************************/

#import "VLCSimplePrefsWindow.h"

#import "preferences/VLCSimplePrefsController.h"

@implementation VLCSimplePrefsWindow

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)changeFont:(id)sender
{
    [[[VLCMain sharedInstance] simplePreferences] changeFont: sender];
}
@end

@implementation VLCSimplePrefsClipView

- (BOOL)isFlipped
{
    return YES;
}

@end
