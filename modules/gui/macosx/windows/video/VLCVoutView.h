// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCVoutView.h: MacOS X video output module
 *****************************************************************************
 * Copyright (C) 2002-2013 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <hartman at videolan dot org>
 *          Eric Petit <titer@m0k.org>
 *          Benjamin Pracht <bigben at videolan dot org>
 *          Pierre d'Herbemont <pdherbemont # videolan org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *          David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import <vlc_vout.h>

/*****************************************************************************
 * VLCVoutView interface
 *****************************************************************************/
@interface VLCVoutView : NSView

@property (readwrite, assign) vout_thread_t * voutThread;

- (void)releaseVoutThread;

@end
