// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * NSView+VLCAdditions.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2013-2019 VLC authors and VideoLAN
 *
 * Authors: David Fuhrmann <dfuhrmann # videolan.org>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *****************************************************************************/

#import "NSView+VLCAdditions.h"

#import "main/CompatibilityFixes.h"

@implementation NSView (VLCAdditions)

+ (instancetype)fromNibNamed:(NSString *)nibName withClass:(Class)viewClass withOwner:(id)owner
{
    /* the following code saves us an instance of NSViewController which we don't need */
    NSNib *nib = [[NSNib alloc] initWithNibNamed:nibName bundle:nil];
    NSArray *topLevelObjects;
    if (![nib instantiateWithOwner:owner topLevelObjects:&topLevelObjects]) {
        NSAssert(1, @"Failed to load nib file to show view");
        return nil;
    }

    for (id topLevelObject in topLevelObjects) {
        if ([topLevelObject isKindOfClass:viewClass]) {
            return topLevelObject;
            break;
        }
    }

    return nil;
}

- (BOOL)shouldShowDarkAppearance
{
    if (@available(macOS 10.14, *)) {
        return [self.effectiveAppearance.name isEqualToString:NSAppearanceNameDarkAqua] ||
               [self.effectiveAppearance.name isEqualToString:NSAppearanceNameVibrantDark];
    }

    return NO;
}

- (void)enableSubviews:(BOOL)enabled
{
    for (NSView *view in [self subviews]) {
        [view enableSubviews:enabled];

        // enable NSControl
        if ([view respondsToSelector:@selector(setEnabled:)]) {
            [(NSControl *)view setEnabled:enabled];
        }
        // also "enable / disable" text views
        if ([view respondsToSelector:@selector(setTextColor:)]) {
            if (enabled == NO) {
                [(NSTextField *)view setTextColor:[NSColor disabledControlTextColor]];
            } else {
                [(NSTextField *)view setTextColor:[NSColor controlTextColor]];
            }
        }

    }
}

@end
