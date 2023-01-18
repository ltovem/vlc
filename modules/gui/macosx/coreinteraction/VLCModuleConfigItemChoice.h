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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCModuleConfigItem;

@interface VLCModuleConfigItemChoice : NSObject

///< Name of the choice item (localized)
@property (readonly, nullable) NSString *name;

///< Description of the choice item (Name followed by its value in brackets)
@property(readonly, copy) NSString *description;

///< Value of the choice item
@property (readonly) NSObject *value;

@property (readonly, nullable) NSString *stringValue;
@property (readonly, nullable) NSNumber *numberValue;


- (instancetype)init NS_UNAVAILABLE;

+ (NSArray<VLCModuleConfigItemChoice *> *)choiceItemsForOption:(VLCModuleConfigItem *)option;

@end

NS_ASSUME_NONNULL_END
