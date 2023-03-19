// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCAddonListItem.m: Addons manager for the Mac
 ****************************************************************************
 * Copyright (C) 2014 VideoLAN and authors
 * Authors:       Felix Paul Kühne <fkuehne # videolan.org>
 *                David Fuhrmann <dfuhrmann # videolan.org>
 *****************************************************************************/

#import "VLCAddonListItem.h"

#import "main/VLCMain.h"
#import "extensions/NSString+Helpers.h"

@interface VLCAddonListItem ()
{
    addon_entry_t *p_addon_entry;
}
@end

@implementation VLCAddonListItem

- (id)initWithAddon:(addon_entry_t *)p_entry
{
    self = [super init];
    if(self) {
        p_addon_entry = addon_entry_Hold(p_entry);
    }

    return self;
}

-(void)dealloc
{
    addon_entry_Release(p_addon_entry);
}

- (NSData *)uuid
{
    vlc_mutex_lock(&p_addon_entry->lock);
    NSData *o_uuid = [NSData dataWithBytes:p_addon_entry->uuid length:sizeof(p_addon_entry->uuid)];
    vlc_mutex_unlock(&p_addon_entry->lock);

    return o_uuid;
}

- (NSString *)name
{
    vlc_mutex_lock(&p_addon_entry->lock);
    NSString *o_str = toNSStr(p_addon_entry->psz_name);
    vlc_mutex_unlock(&p_addon_entry->lock);

    return o_str;
}
- (NSString *)author
{
    vlc_mutex_lock(&p_addon_entry->lock);
    NSString *o_str = toNSStr(p_addon_entry->psz_author);
    vlc_mutex_unlock(&p_addon_entry->lock);

    return o_str;
}

- (NSString *)version
{
    vlc_mutex_lock(&p_addon_entry->lock);
    NSString *o_str = toNSStr(p_addon_entry->psz_version);
    vlc_mutex_unlock(&p_addon_entry->lock);

    return o_str;
}

- (NSString *)description
{
    vlc_mutex_lock(&p_addon_entry->lock);
    NSString *o_str = toNSStr(p_addon_entry->psz_description);
    vlc_mutex_unlock(&p_addon_entry->lock);

    return o_str;
}

- (BOOL)isInstalled
{
    vlc_mutex_lock(&p_addon_entry->lock);
    BOOL b_installed = p_addon_entry->e_state == ADDON_INSTALLED;
    vlc_mutex_unlock(&p_addon_entry->lock);

    return b_installed;
}

- (addon_type_t)type
{
    vlc_mutex_lock(&p_addon_entry->lock);
    addon_type_t type = p_addon_entry->e_type;
    vlc_mutex_unlock(&p_addon_entry->lock);

    return type;
}

@end
