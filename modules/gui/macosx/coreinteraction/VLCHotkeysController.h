// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCHotkeysController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2002-2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan dot org>
 *          David Fuhrmann <dfuhrmann # videolan dot org>
 *          Derk-Jan Hartman <hartman # videolan dot org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import <vlc_vout.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCHotkeysController : NSObject

- (BOOL)handleVideoOutputKeyDown:(id)anEvent forVideoOutput:(vout_thread_t *)p_vout;
- (BOOL)performKeyEquivalent:(NSEvent *)anEvent;

@end

NS_ASSUME_NONNULL_END
