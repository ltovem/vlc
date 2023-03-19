// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryNavigationStack.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCLibraryWindow;

@interface VLCLibraryNavigationStack : NSObject

@property (readwrite, nonatomic) VLCLibraryWindow *delegate;
@property (readonly) BOOL forwardsAvailable;
@property (readonly) BOOL backwardsAvailable;

- (void)backwards;
- (void)forwards;

- (void)appendCurrentLibraryState;
- (void)clear;

@end

NS_ASSUME_NONNULL_END
