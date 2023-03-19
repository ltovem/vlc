// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * VLCMinimalVoutWindow.h: macOS minimal vout window
 *****************************************************************************
 * Copyright (C) 2007-2017 VLC authors and VideoLAN
 *
 * Authors: Pierre d'Herbemont <pdherbemont # videolan.org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCMinimalVoutWindow : NSWindow
{
    NSRect initialFrame;
}

- (id)initWithContentRect:(NSRect)contentRect;

- (void)enterFullscreen;
- (void)leaveFullscreen;
@end
