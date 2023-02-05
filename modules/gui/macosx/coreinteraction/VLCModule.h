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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#import <Foundation/Foundation.h>

#import <vlc_common.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCModuleConfigItem;

@interface VLCModule : NSObject

///< The underlying  VLC module that this object wraps
@property (readonly) const module_t *vlcModule;

///< Internal module name
@property (readonly) NSString *name;

///< Friendly (human-readable) name
@property (readonly) NSString *displayName;

///< Friendly (human-readable) long name
@property (readonly) NSString *displayLongName;

///< Help text (human-readable) for this module
@property (readonly) NSString *helpText;

///< Configuration items for this module
@property (readonly) NSArray<VLCModuleConfigItem *> *configItems;

///< Indicates if this is the main (core) module
@property (readonly) BOOL isMain;

- (instancetype)init NS_UNAVAILABLE;

- (instancetype)initWithModule:(const module_t *)vlcModule;

///< A list of available VLC modules
+ (NSArray<VLCModule *> *)moduleList;

@end

NS_ASSUME_NONNULL_END
