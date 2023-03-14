/*****************************************************************************
 * VLCPlaylistMenuController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2002-2019 VLC authors and VideoLAN
 *
 * Authors: Benjamin Pracht <bigben at videolan dot org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *          David Fuhrmann <dfuhrmann # videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCPlaylistMenuController : NSObject

@property (readwrite, weak) NSTableView *playlistTableView;
@property (readonly) NSMenu *playlistMenu;

@end

NS_ASSUME_NONNULL_END
