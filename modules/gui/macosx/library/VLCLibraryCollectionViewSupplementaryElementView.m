// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewSupplementaryElementView.m: MacOS X interface module
 ******************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 ******************************************************************************/

#import "VLCLibraryCollectionViewSupplementaryElementView.h"
#import "extensions/NSFont+VLCAdditions.h"
#import "extensions/NSColor+VLCAdditions.h"
#import "extensions/NSView+VLCAdditions.h"

NSString *VLCLibrarySupplementaryElementViewIdentifier = @"VLCLibrarySupplementaryElementViewIdentifier";

@implementation VLCLibraryCollectionViewSupplementaryElementView

+ (CGSize)defaultHeaderSize
{
    return CGSizeMake(100., 40.);
}

- (instancetype)initWithFrame:(NSRect)frameRect
{
    self = [super initWithFrame:frameRect];
    if (self) {
        self.font = [NSFont VLClibrarySectionHeaderFont];
        self.textColor = self.shouldShowDarkAppearance ? [NSColor VLClibraryDarkTitleColor] : [NSColor VLClibraryLightTitleColor];
        self.editable = NO;
        self.selectable = NO;
        self.bordered = NO;
        self.usesSingleLineMode = NO;
        self.drawsBackground = YES;
        self.backgroundColor = [NSColor clearColor];
    }
    return self;
}

- (void)viewDidChangeEffectiveAppearance
{
    self.textColor = self.shouldShowDarkAppearance ? [NSColor VLClibraryDarkTitleColor] : [NSColor VLClibraryLightTitleColor];
}

@end
