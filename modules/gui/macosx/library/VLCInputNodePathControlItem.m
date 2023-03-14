/*****************************************************************************
 * VLCInputNodePathControlItem.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCInputNodePathControlItem.h"

#import "VLCInputItem.h"
#import "VLCLibraryImageCache.h"

@implementation VLCInputNodePathControlItem

- (instancetype)initWithInputNode:(VLCInputNode *)inputNode
{
    self = [super init];
    if (self && inputNode != nil && inputNode.inputItem != nil) {
        _inputNode = inputNode;

        VLCInputItem *inputItem = inputNode.inputItem;
        self.image = [VLCLibraryImageCache thumbnailForInputItem:inputItem];
        self.title = inputItem.name;

        // HACK: We have no way when we get the clicked item from the path control
        // of knowing specifically which input node this path item corresponds to,
        // as the path control returns a copy for clickedPathItem that is not of
        // this class. As a very awkward workaround, lets set the name of the image
        // used here as the MRL of the node's input item
        self.image.name = inputItem.MRL;
    }
    return self;
}

@end
