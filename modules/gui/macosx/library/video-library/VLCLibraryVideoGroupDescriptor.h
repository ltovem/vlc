// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryVideoGroupDescriptor.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "library/video-library/VLCLibraryVideoCollectionViewsStackViewController.h"

typedef NS_ENUM(NSUInteger, VLCLibraryVideoGroup) {
    VLCLibraryVideoInvalidGroup = 0,
    VLCLibraryVideoRecentsGroup,
    VLCLibraryVideoLibraryGroup,
    VLCLibraryVideoSentinel,
};

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryVideoCollectionViewGroupDescriptor : NSObject

@property (readonly) VLCLibraryVideoGroup group;
@property (readonly) SEL libraryModelDataSelector;
@property (readonly) NSMethodSignature *libraryModelDataMethodSignature;
@property (readonly) NSNotificationName libraryModelUpdatedNotificationName;
@property (readonly) NSString *name;
@property (readonly) BOOL isHorizontalBarCollectionView;

- (instancetype)initWithVLCVideoLibraryGroup:(VLCLibraryVideoGroup)group;

@end

NS_ASSUME_NONNULL_END
