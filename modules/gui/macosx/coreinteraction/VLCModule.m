/* VLCModule: Represents a VLC module
 *
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat # videolan dot org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 */

#import "VLCModule.h"
#import "VLCModuleConfigItem.h"
#import "extensions/NSString+Helpers.h"

#import <vlc_modules.h>

@implementation VLCModule

@synthesize helpText = _helpText;

- (instancetype)initWithModule:(const module_t *)vlcModule
{
    self = [super init];
    if (!self) {
        return nil;
    }
    
    _vlcModule = vlcModule;
    _name = [NSString stringWithUTF8String:module_get_object(_vlcModule)];
    _displayName = [NSString stringWithNullableCString:module_GetShortName(_vlcModule)];
    _displayLongName = [NSString stringWithNullableCString:module_GetLongName(_vlcModule)];

    _helpText = [NSString stringWithNullableCString:module_get_help(_vlcModule)];
    _isMain = module_is_main(_vlcModule) ? YES : NO;
    
    return self;
}

- (NSString *)description
{
    return [NSString stringWithFormat:@"%@ (%@)", _displayName, _name];
}

+ (NSArray<VLCModule *> *)moduleList
{
    size_t module_count = 0;
    module_t **modules = module_list_get(&module_count);

    NSMutableArray<VLCModule *> *list = [NSMutableArray arrayWithCapacity:module_count];
    for (size_t i = 0; i < module_count; i++) {
        VLCModule *module = [[VLCModule alloc] initWithModule:modules[i]];
        [list addObject:module];
    }

    module_list_free(modules);
    return list;
}

- (NSArray<VLCModuleConfigItem *> *)configItems
{
    unsigned item_count = 0;
    module_config_t *config_items = module_config_get(_vlcModule, &item_count);

    NSMutableArray<VLCModuleConfigItem *> *list = [NSMutableArray arrayWithCapacity:item_count];
    for (unsigned i = 0; i < item_count; i++) {
        VLCModuleConfigItem *item = [[VLCModuleConfigItem alloc] initWithConfigItem:config_items[i] forModule:self];
        [list addObject:item];
    }

    module_config_free(config_items);
    return list;
}

@end
