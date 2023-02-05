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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#import <Foundation/Foundation.h>

#import <vlc_common.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCModule;
@class VLCModuleConfigItemChoice;

@interface VLCModuleConfigItem : NSObject

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithConfigItem:(module_config_t)item
                         forModule:(nullable VLCModule *)mod;

///< The configuration option name
@property (nonatomic, readonly, nullable) NSString *name;

/**
 The option name prefixed with the module name
 
 If the option name already starts prefixed with the module name, this prefix
 is stripped. This is mostly used to display the options in the expert preferences
 in a concise way.
 */
@property (nonatomic, readonly, nullable) NSString *displayName;

///< A short description (human-readable) of the option
@property (nonatomic, readonly, nullable) NSString *description;

///< Option description prefixed by the module name
@property (nonatomic, readonly, nullable) NSString *displayDescription;

///< A long description (human-readable) of the option
@property (nonatomic, readonly, nullable) NSString *longDescription;

///< Indicates if the item is just a hint (category, subcategory, …) and not an option
@property (nonatomic, readonly) BOOL isHint;

///< Indicates if the option value matches its default value
@property (nonatomic, readonly) BOOL isDefault;

///< The VLC module this option belongs to
@property (nonatomic, readonly, nullable) VLCModule *vlcModule;

///< Indicates if the option was modified from its saved state
@property (nonatomic, readonly) BOOL isModified;

///< The type of this values option
@property (nonatomic, readonly) int type;

///< The type name of this values option
@property (nonatomic, readonly) NSString *typeName;

///< The default value of this option
@property (nonatomic, readonly) NSObject *defaultValue;

///< Convenience property to access the default value as string, if applicable
@property (nonatomic, readonly, nullable) NSString *defaultStringValue;

///< Convenience property to access the default value as number, if applicable
@property (nonatomic, readonly, nullable) NSNumber *defaultNumberValue;

/**
 The current value of the option
 
 Setting this value does not actually change the option in the config,
 that only happens once `apply` is called.
 */
@property (atomic, readwrite, nullable) NSObject *value;

///< Convenience property to access the value as string, if applicable
@property (readwrite, nullable) NSString *stringValue;

///< Convenience property to access the value as number, if applicable
@property (readwrite, nullable) NSNumber *numberValue;

///< Convenience property to access the value as a `VLCModuleConfigItemChoice`, if applicable
@property (readwrite, nullable) VLCModuleConfigItemChoice *choiceValue;

///< The smallest accepted number value for this option
@property (nonatomic, nullable, readonly) NSNumber *minValue;

///< The largest accepted number value for this option
@property (nonatomic, nullable, readonly) NSNumber *maxValue;

///< The possible choice values that this option can be set to
@property (nonatomic, nullable, readonly) NSArray<VLCModuleConfigItemChoice *> *choices;

/**
 Save the modified configuration option

 If the option was not modified, this has no effect.
 */
- (void)apply;

@end

NS_ASSUME_NONNULL_END
