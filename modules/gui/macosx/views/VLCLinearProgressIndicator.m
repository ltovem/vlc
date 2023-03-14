/*****************************************************************************
 * VLCLinearProgressIndicator.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2013, 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCLinearProgressIndicator.h"
#import "extensions/NSColor+VLCAdditions.h"

@interface VLCLinearProgressIndicator()
{
    NSView *_foregroundView;
}
@end

@implementation VLCLinearProgressIndicator

- (instancetype)initWithFrame:(NSRect)frameRect
{
    self = [super initWithFrame:frameRect];
    if (self) {
        [self setupSubviews];
    }
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)decoder
{
    self = [super initWithCoder:decoder];
    if (self) {
        [self setupSubviews];
    }
    return self;
}

- (void)setupSubviews
{
    self.wantsLayer = YES;
    self.layer.backgroundColor = [NSColor VLClibraryProgressIndicatorBackgroundColor].CGColor;

    CGRect frame = self.frame;
    frame.size.width = 0.;
    _foregroundView = [[NSView alloc] initWithFrame:frame];
    _foregroundView.wantsLayer = YES;
    _foregroundView.layer.backgroundColor = [NSColor VLClibraryProgressIndicatorForegroundColor].CGColor;
    _foregroundView.autoresizingMask = NSViewWidthSizable;
    _foregroundView.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:_foregroundView];
}

- (void)setProgress:(CGFloat)progress
{
    if (progress > 1.) {
        progress = 1.;
    }

    CGRect rect = self.frame;
    rect.size.width = rect.size.width * progress;
    _foregroundView.frame = rect;

    _progress = progress;
}

@end
