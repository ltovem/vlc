// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryVideoGroupDescriptor.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import "VLCLibraryVideoGroupDescriptor.h"

#import "extensions/NSString+Helpers.h"

#import "library/VLCLibraryModel.h"

@implementation VLCLibraryVideoCollectionViewGroupDescriptor

- (instancetype)initWithVLCVideoLibraryGroup:(VLCLibraryVideoGroup)group
{
    self = [super init];

    if (self) {
        _group = group;

        switch (_group) {
            case VLCLibraryVideoRecentsGroup:
                _libraryModelUpdatedNotificationName = VLCLibraryModelRecentMediaListUpdated;
                _libraryModelDataSelector = @selector(listOfRecentMedia);
                _isHorizontalBarCollectionView = YES;
                _name = _NS("Recents");
                break;
            case VLCLibraryVideoLibraryGroup:
                _libraryModelUpdatedNotificationName = VLCLibraryModelVideoMediaListUpdated;
                _libraryModelDataSelector = @selector(listOfVideoMedia);
                _isHorizontalBarCollectionView = NO;
                _name = _NS("Library");
                break;
            default:
                NSAssert(1, @"Cannot construct group descriptor from invalid VLCLibraryVideoGroup value");
                _group = VLCLibraryVideoInvalidGroup;
                break;
        }

        _libraryModelDataMethodSignature = [VLCLibraryModel instanceMethodSignatureForSelector:_libraryModelDataSelector];
    }

    return self;
}

@end
