//
//  VLCHUDTableCornerView.m
//  BGHUDAppKit
//
//  Created by BinaryGod on 6/29/08.
//
//  Copyright (c) 2008, Tim Davis (BinaryMethod.com, binary.god@gmail.com)
//  All rights reserved.
//
//  SPDX-License-Identifier: BSD-3-Clause
#import "VLCHUDTableCornerView.h"

@implementation VLCHUDTableCornerView

- (instancetype)init
{
    self = [super init];

    if (self) {
        _tableHeaderCellBorderColor = [NSColor colorWithDeviceRed:0.349f green:0.361f blue:0.388f alpha:1.0f];
        _tableHeaderCellNormalFill = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithDeviceRed:0.251f green:0.251f blue:0.255f alpha:1.0f]
                                                                   endingColor:[NSColor colorWithDeviceRed:0.118f green:0.118f blue:0.118f alpha:1.0f]];
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {

    // Draw base layer
    [_tableHeaderCellBorderColor set];
    NSRectFill([self bounds]);

    [_tableHeaderCellNormalFill drawInRect:NSInsetRect([self bounds], 1, 1) angle:270];
}

@end
