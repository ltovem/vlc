// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCPlaylistSortingMenuController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2002-2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan dot org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCPlaylistSortingMenuController : NSObject

@property (readonly) NSMenu *playlistSortingMenu;

@end

NS_ASSUME_NONNULL_END
