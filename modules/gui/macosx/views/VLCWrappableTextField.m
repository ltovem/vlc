/*****************************************************************************
 * VLCWrappableTextField.m
 *****************************************************************************
 * Copyright (C) 2017 VideoLAN and authors
 * Author:       David Fuhrmann <dfuhrmann at videolan dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCWrappableTextField.h"


@implementation VLCWrappableTextField

- (NSSize)intrinsicContentSize
{
    if (![self.cell wraps]) {
        return [super intrinsicContentSize];
    }

    // Try to get minimum height needed, by assuming unlimited height being
    // (theoretically) possible.
    NSRect frame = [self frame];
    frame.size.height = CGFLOAT_MAX;

    CGFloat height = [self.cell cellSizeForBounds:frame].height;

    return NSMakeSize(frame.size.width, height);
}

- (void)textDidChange:(NSNotification *)notification
{
    [super textDidChange:notification];
    [self invalidateIntrinsicContentSize];
}

@end
