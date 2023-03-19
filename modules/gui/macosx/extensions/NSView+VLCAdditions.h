// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * NSView+VLCAdditions.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2013-2019 VLC authors and VideoLAN
 *
 * Authors: David Fuhrmann <dfuhrmann # videolan.org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSView (VLCAdditions)

+ (instancetype)fromNibNamed:(NSString *)nibName withClass:(Class)viewClass withOwner:(id)owner;

@property (readonly) BOOL shouldShowDarkAppearance;

- (void)enableSubviews:(BOOL)enabled;

@end

NS_ASSUME_NONNULL_END
