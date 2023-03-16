// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCImageView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

typedef NS_ENUM(NSInteger, VLCImageViewContentGravity) {
    VLCImageViewContentGravityCenter,
    VLCImageViewContentGravityTop,
    VLCImageViewContentGravityBottom,
    VLCImageViewContentGravityLeft,
    VLCImageViewContentGravityRight,
    VLCImageViewContentGravityTopLeft,
    VLCImageViewContentGravityTopRight,
    VLCImageViewContentGravityBottomLeft,
    VLCImageViewContentGravityBottomRight,
    VLCImageViewContentGravityResize,
    VLCImageViewContentGravityResizeAspect,
    VLCImageViewContentGravityResizeAspectFill,
};

@interface VLCImageView : NSView

@property (readwrite, retain, nonatomic, nullable) NSImage *image;
@property (readwrite) VLCImageViewContentGravity contentGravity;
@property (readwrite) BOOL cropsImagesToRoundedCorners;

- (void)setImageURL:(NSURL * _Nonnull)url placeholderImage:(NSImage * _Nullable)image;

@end
