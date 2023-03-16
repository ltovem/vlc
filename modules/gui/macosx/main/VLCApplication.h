// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCApplication.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2002-2016 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <hartman at videolan dot org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *          David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *          Pierre d'Herbemont <pdherbemont # videolan org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

/*****************************************************************************
 * VLCApplication interface
 *****************************************************************************/

@interface VLCApplication : NSApplication

/**
 * The current VLC App icon image
 *
 * This is adjusted accordingly to return the special
 * image on occasions like christmas. Contrary to the
 * applicationIconImage property though, the image is
 * not scaled down, so it remains suitable when it is
 * displayed for example in the About window.
 *
 * Must be called from the main thread only.
 */
@property(strong, readonly) NSImage *vlcAppIconImage;

@end
