// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCRoundedCornerTextField.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import "VLCRoundedCornerTextField.h"
#import "extensions/NSColor+VLCAdditions.h"

const CGFloat VLCRoundedCornerTextFieldLightCornerRadius = 3.;
const CGFloat VLCRoundedCornerTextFieldStrongCornerRadius = 10.;

@implementation VLCRoundedCornerTextField

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        [self setupCustomAppearance];
    }
    return self;
}

- (instancetype)initWithFrame:(NSRect)frameRect
{
    self = [super initWithFrame:frameRect];
    if (self) {
        [self setupCustomAppearance];
    }
    return self;
}

- (void)setupCustomAppearance
{
    self.wantsLayer = YES;
    self.layer.cornerRadius = VLCRoundedCornerTextFieldLightCornerRadius;
    self.layer.masksToBounds = YES;
    self.layer.backgroundColor = [NSColor VLClibraryAnnotationBackgroundColor].CGColor;
}

- (void)setBackgroundColor:(NSColor *)backgroundColor
{
    self.layer.backgroundColor = backgroundColor.CGColor;
}

- (void)setUseStrongRounding:(BOOL)useStrongRounding
{
    _useStrongRounding = useStrongRounding;
    if (_useStrongRounding) {
        self.layer.cornerRadius = VLCRoundedCornerTextFieldStrongCornerRadius;
    } else {
        self.layer.cornerRadius = VLCRoundedCornerTextFieldLightCornerRadius;
    }
}

- (void)setStringValue:(NSString *)stringValue
{
    if (stringValue != nil) {
        [super setStringValue:[NSString stringWithFormat:@" %@ ", stringValue]];
    } else {
        [super setStringValue:@""];
    }
}

@end
