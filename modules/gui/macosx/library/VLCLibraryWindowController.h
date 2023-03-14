/*****************************************************************************
 * VLCLibraryWindowController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryWindowController : NSWindowController<NSWindowRestoration>

- (instancetype)initWithLibraryWindow;

@end

NS_ASSUME_NONNULL_END
