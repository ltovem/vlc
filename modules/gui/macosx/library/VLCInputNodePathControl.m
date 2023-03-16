// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCInputNodePathControl.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import "VLCInputNodePathControl.h"

#import "VLCInputNodePathControlItem.h"

@implementation VLCInputNodePathControl

- (void)appendInputNodePathControlItem:(VLCInputNodePathControlItem *)inputNodePathControlItem
{
    if (_inputNodePathControlItems == nil) {
        _inputNodePathControlItems = [NSMutableDictionary dictionary];
    }

    if ([inputNodePathControlItem.image.name isEqualToString:@""]) {
        return;
    }

    [_inputNodePathControlItems setObject:inputNodePathControlItem forKey:inputNodePathControlItem.image.name];

    NSMutableArray *pathItems = [NSMutableArray arrayWithArray:self.pathItems];
    [pathItems addObject:inputNodePathControlItem];
    self.pathItems = pathItems;
}

- (void)removeLastInputNodePathControlItem
{
    if (self.pathItems.count == 0) {
        _inputNodePathControlItems = [NSMutableDictionary dictionary];
        return;
    }

    NSMutableArray *pathItems = [NSMutableArray arrayWithArray:self.pathItems];
    NSPathControlItem *lastItem = pathItems.lastObject;

    [pathItems removeLastObject];
    self.pathItems = pathItems;
    [_inputNodePathControlItems removeObjectForKey:lastItem.image.name];
}

- (void)clearInputNodePathControlItems
{
    _inputNodePathControlItems = [NSMutableDictionary dictionary];
    self.pathItems = @[];
}

- (void)clearPathControlItemsAheadOf:(NSPathControlItem *)item
{
    if ([item.image.name isEqualToString:@""]) {
        return;
    }

    NSUInteger indexOfItem = [self.pathItems indexOfObjectPassingTest:^BOOL(NSPathControlItem *searchItem, NSUInteger idx, BOOL *stop) {
        return [searchItem.image.name isEqualToString:item.image.name];
    }];

    if (indexOfItem == NSNotFound) {
        return;
    }

    NSMutableArray<NSPathControlItem *> *pathItems = [NSMutableArray arrayWithArray:self.pathItems];
    NSArray<NSPathControlItem *> *itemsToRemove = [pathItems subarrayWithRange:NSMakeRange(indexOfItem + 1, pathItems.count - indexOfItem - 1)];
    NSMutableArray<NSString *> *itemMrlsToRemove = [NSMutableArray arrayWithCapacity:itemsToRemove.count];

    for (NSPathControlItem *searchItem in itemsToRemove) {
        NSString *searchItemMrl = searchItem.image.name;
        [itemMrlsToRemove addObject:searchItemMrl];
    };

    self.pathItems = [pathItems subarrayWithRange:NSMakeRange(0, indexOfItem + 1)];
    [_inputNodePathControlItems removeObjectsForKeys:itemMrlsToRemove];
}

@end
