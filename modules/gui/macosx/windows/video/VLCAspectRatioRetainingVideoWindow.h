/*****************************************************************************
 * VLCAspectRatioRetainingVideoWindow.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCFullVideoViewWindow.h"

NS_ASSUME_NONNULL_BEGIN

@interface VLCAspectRatioRetainingVideoWindow : VLCFullVideoViewWindow

@property (nonatomic, readwrite, assign) NSSize nativeVideoSize;

@end

NS_ASSUME_NONNULL_END
