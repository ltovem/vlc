/*****************************************************************************
 * VLCAddonListItem.h: Addons manager for the Mac
 ****************************************************************************
 * Copyright (C) 2014 VideoLAN and authors
 * Authors:       Felix Paul Kühne <fkuehne # videolan.org>
 *                David Fuhrmann <dfuhrmann # videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import <vlc_common.h>
#import <vlc_addons.h>

@interface VLCAddonListItem : NSObject

- (id)initWithAddon:(addon_entry_t *)p_entry;

- (NSData *)uuid;

- (NSString *)name;
- (NSString *)author;
- (NSString *)version;
- (NSString *)description;

- (BOOL)isInstalled;
- (addon_type_t)type;

@end
