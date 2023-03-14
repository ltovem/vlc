/*****************************************************************************
 * VLCLibrarySortingMenuController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibrarySortingMenuController : NSObject

@property (readonly) NSMenu *librarySortingMenu;

@end

NS_ASSUME_NONNULL_END
