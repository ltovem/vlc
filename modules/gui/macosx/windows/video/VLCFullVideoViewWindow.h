// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCFullVideoViewWindow: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import "VLCVideoWindowCommon.h"

NS_ASSUME_NONNULL_BEGIN

@interface VLCFullVideoViewWindow : VLCVideoWindowCommon

- (void)stopTitlebarAutohideTimer;
- (void)startTitlebarAutohideTimer;
- (void)showTitleBar;

- (void)hideTitleBar:(id)sender;
- (void)enableVideoTitleBarMode;
- (void)disableVideoTitleBarMode;

@end

NS_ASSUME_NONNULL_END
