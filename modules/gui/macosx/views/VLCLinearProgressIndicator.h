// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLinearProgressIndicator.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2013, 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCLinearProgressIndicator : NSView

@property (nonatomic) CGFloat progress;

@end

NS_ASSUME_NONNULL_END
