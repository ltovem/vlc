/* VLCModuleConfigItem: Represents a VLC module configuration item
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

#import "VLCModuleConfigItem.h"

#import <vlc_configuration.h>
#import <vlc_plugin.h>

#import "VLCModule.h"
#import "VLCModuleConfigItemChoice.h"
#import "extensions/NSString+Helpers.h"
#import "extensions/NSColor+VLCAdditions.h"

@interface VLCModuleConfigItem () {}
@property (atomic) NSObject *oldValue;

+ (NSObject *)objectForModuleValue:(module_value_t)value
                          withType:(int)type __attribute__((objc_direct));
@end

@implementation VLCModuleConfigItem

// Explicit synthesize needed here, as we override
// the superclass' description property.
@synthesize description = _description;
@synthesize choices = _choices;

/**
 * Create an object corresponding to the module value type.
 * Fundamentally a module config item can store three underlying types:
 *  - Integer
 *  - Float
 *  - String
 *
 * The numeric types are mapped to NSNumber, the strings to NSString.
 *
 * - Throws: ``NSInvalidArgumentException`` if the `type` is not handled.
 */
+ (NSObject *)objectForModuleValue:(module_value_t)value withType:(int)type
{
    if (IsConfigStringType(type)) {
        return [NSString stringWithNullableCString:value.psz];
    } else if (IsConfigIntegerType(type)) {
        return [NSNumber numberWithLongLong:value.i];
    } else if (IsConfigFloatType(type)) {
        return [NSNumber numberWithFloat:value.f];
    }

    @throw [NSException exceptionWithName:NSInvalidArgumentException
                                   reason:@"Unhandled module_config_t type"
                                 userInfo:@{ @"unhandledTypeValue" : @(type) }];
}

/*
 * Returns a Set containing keyPaths of properties that affect another property.
 *
 * For example for colorValue it should return ['value', 'numberValue']
 * as these properties affect the colorValue.
 */
+ (NSSet<NSString *> *)keyPathsForValuesAffectingValueForKey:(NSString *)key
{
    NSArray *affectingKeys = nil;
    NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];

    // The keyPaths of all 'convenience' accessors that
    // end up accessing/setting the value property.
    // These MUST set the value though the property and
    // never not use _value to change it!
    NSArray<NSString *> *convenienceAccessors = @[
        @"stringValue", @"numberValue", @"colorValue", @"choiceValue"
    ];
    // Read-only properties that depend on value
    NSArray<NSString *> *valueDependants = @[
        @"isDefault", @"isModified"
    ];

    if ([key isEqualToString:@"value"]) {
        // Value is affected by all other 'convenience' values
        affectingKeys = convenienceAccessors;
    } else if ([convenienceAccessors containsObject:key] ||
               [valueDependants containsObject:key]) {
        // Any convenience accessor is affected by the value
        affectingKeys = @[@"value"];
    }
    // We do not need to list the dependencies between the
    // various convenience accessors, like that colorValue
    // and numberValue affect each other, because the setters
    // for these end up setting the value through the property
    // which triggers a change notification for value which in
    // turns all the other convenience accessors depend on.
    
    // isModified is additionally affected by the oldValue
    if ([key isEqualToString:@"isModified"]) {
        affectingKeys = [affectingKeys arrayByAddingObject:@"oldValue"];
    }

    if (affectingKeys)
        keyPaths = [keyPaths setByAddingObjectsFromArray:affectingKeys];
    return keyPaths;
}

- (instancetype)initWithConfigItem:(module_config_t)item
                         forModule:(nullable VLCModule *)mod
{
    self = [super init];
    if (!self) {
        return nil;
    }

    _vlcModule = mod;
    _name = [NSString stringWithNullableCString:item.psz_name];
    _description = [NSString stringWithNullableCString:item.psz_text];
    _longDescription = [NSString stringWithNullableCString:item.psz_longtext];
    _type = item.i_type;

    if (!self.isHint) {
        _value = [VLCModuleConfigItem objectForModuleValue:item.value
                                                  withType:_type];
        _defaultValue = [VLCModuleConfigItem objectForModuleValue:item.orig
                                                         withType:_type];
        // FIXME: We should not get different values here from the core
        // Either both default and value should be NULL or both empty string.
        if (_value == nil)
            _value = _defaultValue;

        if (IsConfigIntegerType(_type) &&
            (item.min.i != INT64_MIN || item.max.i != INT64_MAX )) {
            _minValue = [NSNumber numberWithLongLong:item.min.i];
            _maxValue = [NSNumber numberWithLongLong:item.max.i];
        } else if (IsConfigFloatType(_type) &&
                   (item.min.f != FLT_MIN || item.max.f != FLT_MAX )) {
            _minValue = [NSNumber numberWithFloat:item.min.f];
            _maxValue = [NSNumber numberWithFloat:item.max.f];
        }
    }

    _oldValue = _value;
    return self;
}

- (NSArray<VLCModuleConfigItemChoice *> *)choices
{
    if (_choices == nil)
        _choices = [VLCModuleConfigItemChoice choiceItemsForOption:self];
    return _choices;
}

- (NSString *)displayName
{
    // Create display name from dot-prefixing module name to option name.
    // Many plugins dash-prefix their name to their option names, which we must
    // skip over here to replace the dash with a dot.
    if (!_name || !_vlcModule.name || _vlcModule.isMain)
        return _name;

    NSString *finalString = _name;
    NSString *modulePrefix = [NSString stringWithFormat:@"%@-", _vlcModule.name];
    if ([finalString hasPrefix:_vlcModule.name] &&
        _vlcModule.name.length > finalString.length &&
        [finalString characterAtIndex:_vlcModule.name.length + 1] == (unichar)'-') {
        finalString = [finalString substringFromIndex:_vlcModule.name.length + 1];
    }
    finalString = [NSString stringWithFormat:@"%@.%@", _vlcModule.name, finalString];
    finalString = [finalString stringByReplacingOccurrencesOfString:@"_" withString:@"-"];
    return finalString;
}

- (NSString *)displayDescription
{
    if (!_description || !_vlcModule.displayName)
        return _description;
    return [NSString stringWithFormat:@"%@ :: %@", _vlcModule.displayName, _description];
}

- (BOOL)isHint
{
    return (CONFIG_ITEM(_type)) ? NO : YES;
}

- (BOOL)isDefault
{
    return (_value == _defaultValue ||
            [_value isEqual:_defaultValue]);
}

- (BOOL)isModified
{
    return (_value != _oldValue &&
            ![_value isEqual:_oldValue]);
}

- (NSString *)typeName
{
    switch (_type) {
        case CONFIG_HINT_CATEGORY:
            return @"Hint (Category)";
        case CONFIG_SUBCATEGORY:
            return @"Hint (Subcategory)";
        case CONFIG_SECTION:
            return @"Hint (Section)";
        case CONFIG_ITEM_FLOAT:
            return @"Float";
        case CONFIG_ITEM_INTEGER:
            return @"Integer";
        case CONFIG_ITEM_RGB:
            return @"RGB Color";
        case CONFIG_ITEM_BOOL:
            return @"Boolean";
        case CONFIG_ITEM_STRING:
            return @"String";
        case CONFIG_ITEM_PASSWORD:
            return @"Password";
        case CONFIG_ITEM_KEY:
            return @"Shortcut";
        case CONFIG_ITEM_MODULE:
        case CONFIG_ITEM_MODULE_CAT:
            return @"Module";
        case CONFIG_ITEM_MODULE_LIST:
        case CONFIG_ITEM_MODULE_LIST_CAT:
            return @"Module list";
        case CONFIG_ITEM_LOADFILE:
        case CONFIG_ITEM_SAVEFILE:
            return @"File";
        case CONFIG_ITEM_DIRECTORY:
            return @"Directory";
        case CONFIG_ITEM_FONT:
            return @"Font";
            
        default:
            return @"Unknown";
    }
}

- (NSString *)defaultStringValue
{
    if (!IsConfigStringType(_type))
        return nil;

    return (NSString *)_defaultValue;
}

- (NSNumber *)defaultNumberValue
{
    if (!IsConfigIntegerType(_type) && !IsConfigFloatType(_type))
        return nil;

    return (NSNumber *)_defaultValue;
}

- (NSString *)stringValue
{
    if (!IsConfigStringType(_type))
        return nil;
    return (NSString *)_value;
}

- (void)setStringValue:(NSString *)stringValue
{
    if (!IsConfigStringType(_type)) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException
                                       reason:@"Option does not accept string values"
                                     userInfo:nil];
    }
    self.value = stringValue;
}

- (NSNumber *)numberValue
{
    if (!IsConfigIntegerType(_type) && !IsConfigFloatType(_type))
        return nil;

    return (NSNumber *)_value;
}

- (void)setNumberValue:(NSNumber *)numberValue
{
    if (!IsConfigIntegerType(_type) && !IsConfigFloatType(_type)) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException
                                       reason:@"Option does not accept number values"
                                     userInfo:nil];
    }
    self.value = numberValue;
}

- (NSColor *)colorValue
{
    if (_type != CONFIG_ITEM_RGB) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException
                                       reason:@"Option does not accept RGB color values"
                                     userInfo:nil];
    }

    unsigned long rgbValue = self.numberValue.longValue;
    uint8_t r = (rgbValue & 0xFF0000) >> 16;
    uint8_t g = (rgbValue & 0x00FF00) >> 8;
    uint8_t b = (rgbValue & 0x0000FF);
    return [NSColor colorWithSRGBBytesRed:r green:g blue:b];
}

- (void)setColorValue:(NSColor *)colorValue
{
    if (_type != CONFIG_ITEM_RGB) {
        @throw [NSException exceptionWithName:NSInvalidArgumentException
                                       reason:@"Option does not accept RGB color values"
                                     userInfo:nil];
    }

    uint8_t r, g, b;

    // Convert to RGB color space first, in case the color comes from a
    // color picker it could be in a different one and getting the components
    // would cause an exception.
    NSColor *rgbColor = [colorValue colorUsingColorSpace:NSColorSpace.sRGBColorSpace];
    [rgbColor getBytesRed:&r green:&g blue:&b];
    uint32_t rgb = (r << 16) + (g << 8) + (b);

    self.value = [NSNumber numberWithUnsignedLong:rgb];
}

- (VLCModuleConfigItemChoice *)choiceValue
{
    for (VLCModuleConfigItemChoice *choice in self.choices) {
        if ([choice.value isEqual:self.value])
            return choice;
    }
    return nil;
}

- (void)setChoiceValue:(VLCModuleConfigItemChoice *)choiceValue
{
    self.value = choiceValue.value;
}

- (void)apply
{
    if (!self.isModified)
        return;

    NSLog(@"Applying setting for option %@", self.name);
    if (IsConfigIntegerType(_type)) {
        config_PutInt(_name.UTF8String, self.numberValue.longLongValue);
    } else if (IsConfigFloatType(_type)) {
        config_PutFloat(_name.UTF8String, self.numberValue.floatValue);
    } else if (IsConfigStringType(_type)) {
        config_PutPsz(_name.UTF8String, self.stringValue.UTF8String);
    } else {
        @throw [NSException exceptionWithName:NSInternalInconsistencyException
                                       reason:@"Config item type not handled in -apply"
                                     userInfo:@{ @"unhandledTypeValue" : @(_type) }];
    }
    self.oldValue = self.value;
}

@end
