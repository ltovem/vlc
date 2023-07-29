/*****************************************************************************
 * VLCInputNodePathControl.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
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
 *****************************************************************************/

#import "VLCInputNodePathControl.h"

#import "VLCInputNodePathControlItem.h"

@implementation VLCInputNodePathControl

- (void)appendInputNodePathControlItem:(VLCInputNodePathControlItem *)inputNodePathControlItem
{
    NSString * const inputNodeMRL = [VLCInputNodePathControlItem MRLFromPathControlItem:inputNodePathControlItem];
    NSParameterAssert(![inputNodeMRL isEqualToString:@""]);

    if (_inputNodePathControlItems == nil) {
        _inputNodePathControlItems = NSMutableDictionary.dictionary;
    }

    [_inputNodePathControlItems setObject:inputNodePathControlItem forKey:inputNodeMRL];

    NSMutableArray * const pathItems = self.pathItems.mutableCopy;
    [pathItems addObject:inputNodePathControlItem];
    self.pathItems = pathItems;
}

- (void)removeLastInputNodePathControlItem
{
    if (self.pathItems.count == 0) {
        _inputNodePathControlItems = NSMutableDictionary.dictionary;
        return;
    }

    NSMutableArray * const pathItems = self.pathItems.mutableCopy;
    NSPathControlItem * const lastItem = pathItems.lastObject;
    NSString * const lastItemMRL = [VLCInputNodePathControlItem MRLFromPathControlItem:lastItem];

    [pathItems removeLastObject];
    self.pathItems = pathItems;
    [_inputNodePathControlItems removeObjectForKey:lastItemMRL];
}

- (void)clearInputNodePathControlItems
{
    _inputNodePathControlItems = NSMutableDictionary.dictionary;
    self.pathItems = @[];
}

- (void)clearPathControlItemsAheadOf:(NSPathControlItem *)item
{
    NSString * const itemMRL = [VLCInputNodePathControlItem MRLFromPathControlItem:item];
    if ([itemMRL isEqualToString:@""]) {
        return;
    }

    NSUInteger indexOfItem = [self.pathItems indexOfObjectPassingTest:^BOOL(NSPathControlItem * const searchItem, const NSUInteger idx, BOOL * const stop) {
        NSString * const searchItemMRL = [VLCInputNodePathControlItem MRLFromPathControlItem:searchItem];
        return [searchItemMRL isEqualToString:itemMRL];
    }];

    if (indexOfItem == NSNotFound) {
        return;
    }

    NSMutableArray<NSPathControlItem *> * const pathItems = self.pathItems.mutableCopy;
    NSArray<NSPathControlItem *> * const itemsToRemove = [pathItems subarrayWithRange:NSMakeRange(indexOfItem + 1, pathItems.count - indexOfItem - 1)];
    NSMutableArray<NSString *> * const itemMrlsToRemove = [NSMutableArray arrayWithCapacity:itemsToRemove.count];

    for (NSPathControlItem * const searchItem in itemsToRemove) {
        NSString * const searchItemMrl = [VLCInputNodePathControlItem MRLFromPathControlItem:searchItem];
        [itemMrlsToRemove addObject:searchItemMrl];
    };

    self.pathItems = [pathItems subarrayWithRange:NSMakeRange(0, indexOfItem + 1)];
    [_inputNodePathControlItems removeObjectsForKeys:itemMrlsToRemove];
}

@end
