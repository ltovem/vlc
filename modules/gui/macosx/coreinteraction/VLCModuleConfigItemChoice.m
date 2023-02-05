/* VLCModuleConfigItemChoice: Represents a choice value for a config option
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

#import "VLCModuleConfigItemChoice.h"

#import <vlc_plugin.h>

#import "VLCModuleConfigItem.h"

@interface VLCModuleConfigItemChoice ()
- (instancetype)initWithName:(NSString *)name
                       Value:(NSObject *)value;
@end

@implementation VLCModuleConfigItemChoice

- (instancetype)initWithName:(NSString *)name
                       Value:(NSObject *)value
{
    self = [super init];
    if (!self)
        return nil;

    _name = name;
    _value = value;
    return self;
}

- (NSString *)stringValue
{
    if (![_value isKindOfClass:NSString.class])
        return nil;
    return (NSString *)_value;
}

- (NSNumber *)numberValue
{
    if (![_value isKindOfClass:NSNumber.class])
        return nil;
    return (NSNumber *)_value;
}

- (NSString *)description
{
    if (!_name)
        return _value.description;
    if (_value.description.length == 0)
        return _name;
    return [NSString stringWithFormat:@"%@ (%@)", _name, _value];
}

- (BOOL)isEqual:(id)object
{
    if (![object isKindOfClass:VLCModuleConfigItemChoice.class])
        return NO;
    VLCModuleConfigItemChoice *otherChoice = object;
    return [otherChoice.value isEqual:self.value];
}

+ (NSArray<VLCModuleConfigItemChoice *> *)choiceItemsForOption:(VLCModuleConfigItem *)option;
{
    ssize_t count;
    void *opaqueValues;
    char **descriptions;
    id (^toObject)(size_t i);

    switch (option.type) {
        case CONFIG_ITEM_STRING: {
            char **values;
            count = config_GetPszChoices(option.name.UTF8String, &values, &descriptions);
            toObject = ^id(size_t i) {
                return [[NSString alloc] initWithBytesNoCopy:values[i]
                                                      length:strlen(values[i])
                                                    encoding:NSUTF8StringEncoding
                                                freeWhenDone:YES];
            };
            opaqueValues = values;
            break;
        }
        case CONFIG_ITEM_INTEGER: {
            int64_t *values;
            count = config_GetIntChoices(option.name.UTF8String, &values, &descriptions);
            toObject = ^id(size_t i) {
                return [NSNumber numberWithLongLong:values[i]];
            };
            opaqueValues = values;
            break;
        }
        default:
            return nil;
    }
    if (count <= 0)
        return nil;

    NSMutableArray *items = [NSMutableArray arrayWithCapacity:count];
    for (size_t i = 0; i < count; i++) {
        id valueObject = toObject(i);
        NSString *descStr = [[NSString alloc] initWithBytesNoCopy:descriptions[i]
                                                           length:strlen(descriptions[i])
                                                         encoding:NSUTF8StringEncoding
                                                     freeWhenDone:YES];
        [items addObject:[[VLCModuleConfigItemChoice alloc] initWithName:descStr
                                                                   Value:valueObject]];
    }
    free(opaqueValues);
    free(descriptions);
    return items;
}

@end
