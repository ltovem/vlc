// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCImageView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import "VLCImageView.h"

#import <QuartzCore/QuartzCore.h>

#import "extensions/NSColor+VLCAdditions.h"
#import "extensions/NSView+VLCAdditions.h"

@interface VLCImageView()
{
    NSURL *_currentArtworkURL;
}
@end

@implementation VLCImageView

- (instancetype)initWithFrame:(NSRect)frameRect
{
    self = [super initWithFrame:frameRect];
    if (self) {
        [self setupLayer];
    }
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)decoder
{
    self = [super initWithCoder:decoder];
    if (self) {
        [self setupLayer];
    }
    return self;
}

- (void)setupLayer
{
    self.layer = [[CALayer alloc] init];
    self.contentGravity = VLCImageViewContentGravityResizeAspectFill;
    self.wantsLayer = YES;
    [self setCropsImagesToRoundedCorners:YES];
    [self setupBorderColor];
}

- (void)setCropsImagesToRoundedCorners:(BOOL)cropsImagesToRoundedCorners
{
    if (cropsImagesToRoundedCorners) {
        self.layer.cornerRadius = 5.;
        self.layer.masksToBounds = YES;
        self.layer.borderWidth = 2.;
    } else {
        self.layer.cornerRadius = 0.;
        self.layer.masksToBounds = NO;
        self.layer.borderWidth = 0.;
    }
}

- (BOOL)cropsImagesToRoundedCorners
{
    return self.layer.masksToBounds;
}

- (void)setupBorderColor
{
    if (self.shouldShowDarkAppearance) {
        self.layer.borderColor = [NSColor VLClibraryItemBorderDarkColor].CGColor;
    } else {
        self.layer.borderColor = [NSColor VLClibraryItemBorderLightColor].CGColor;
    }
}

- (void)setImage:(NSImage *)image
{
    _image = image;
    CGFloat desiredScaleFactor = [self.window backingScaleFactor];
    CGFloat actualScaleFactor = [image recommendedLayerContentsScale:desiredScaleFactor];

    id layerContents = [image layerContentsForContentsScale:actualScaleFactor];

    [self setCAContentGravity:_contentGravity];
    [self.layer setContents:layerContents];
    [self.layer setContentsScale:actualScaleFactor];
}

- (void)setCAContentGravity:(VLCImageViewContentGravity)contentGravity
{
    switch (contentGravity) {
        case VLCImageViewContentGravityCenter:
            self.layer.contentsGravity = kCAGravityCenter;
            break;
        case VLCImageViewContentGravityTop:
            self.layer.contentsGravity = kCAGravityTop;
            break;
        case VLCImageViewContentGravityBottom:
            self.layer.contentsGravity = kCAGravityBottom;
            break;
        case VLCImageViewContentGravityLeft:
            self.layer.contentsGravity = kCAGravityLeft;
            break;
        case VLCImageViewContentGravityRight:
            self.layer.contentsGravity = kCAGravityRight;
            break;
        case VLCImageViewContentGravityTopLeft:
            self.layer.contentsGravity = kCAGravityTopLeft;
            break;
        case VLCImageViewContentGravityTopRight:
            self.layer.contentsGravity = kCAGravityTopRight;
            break;
        case VLCImageViewContentGravityBottomLeft:
            self.layer.contentsGravity = kCAGravityBottomLeft;
            break;
        case VLCImageViewContentGravityBottomRight:
            self.layer.contentsGravity = kCAGravityBottomRight;
            break;
        case VLCImageViewContentGravityResize:
            self.layer.contentsGravity = kCAGravityResize;
            break;
        case VLCImageViewContentGravityResizeAspect:
            self.layer.contentsGravity = kCAGravityResizeAspect;
            break;
        case VLCImageViewContentGravityResizeAspectFill:
        default:
            self.layer.contentsGravity = kCAGravityResizeAspectFill;
            break;
    }
}

- (void)viewDidChangeEffectiveAppearance
{
    [self setupBorderColor];
}

- (void)setImageURL:(NSURL * _Nonnull)artworkURL placeholderImage:(NSImage * _Nullable)image
{
    if([_currentArtworkURL isEqual:artworkURL]) {
        return;
    }

    _currentArtworkURL = artworkURL;
    [self setImage:image];

    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        NSImage *downloadedImage = [[NSImage alloc] initWithContentsOfURL:artworkURL];
        dispatch_async(dispatch_get_main_queue(), ^{
            [self setImage:downloadedImage];
        });
    });
}

@end
