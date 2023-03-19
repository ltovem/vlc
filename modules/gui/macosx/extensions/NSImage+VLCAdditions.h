// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * NSImage+VLCAdditions.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSImage (VLCAdditions)

+ (instancetype)quickLookPreviewForLocalPath:(NSString*)path withSize:(NSSize)size;
+ (instancetype)quickLookPreviewForLocalURL:(NSURL*)url withSize:(NSSize)size;

@end

NS_ASSUME_NONNULL_END
