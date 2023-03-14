/*****************************************************************************
 * VLCLibraryNavigationStack.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCLibraryWindow;
@class VLCMediaSourceDataSource;
@class VLCInputNode;
@class VLCMediaSource;

@interface VLCLibraryNavigationState : NSObject

@property (readonly) VLCMediaSourceDataSource *currentMediaSource;
@property (readonly) VLCInputNode *currentNodeDisplayed;

- (instancetype)initFromLibraryWindow:(VLCLibraryWindow *)libraryWindow;

@end

NS_ASSUME_NONNULL_END
