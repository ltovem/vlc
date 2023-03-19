//
//  VLCHUDTableView.m
//  BGHUDAppKit
//
//  Created by BinaryGod on 6/17/08.
//
//  Copyright (c) 2008, Tim Davis (BinaryMethod.com, binary.god@gmail.com)
//  All rights reserved.
//
//  SPDX-License-Identifier: BSD-3-Clause

#import "VLCHUDTableView.h"

#import "views/VLCHUDTableCornerView.h"

@interface NSTableView (private)
- (void)_sendDelegateWillDisplayCell:(id)cell forColumn:(id)column row:(NSInteger)row;
@end

@implementation VLCHUDTableView

#pragma mark Drawing Functions

- (instancetype)initWithCoder:(NSCoder *)decoder
{

    self = [super initWithCoder: decoder];

    if (self) {
        _tableBackgroundColor = [NSColor colorWithCalibratedRed:0 green:0 blue:0 alpha:0];
        _cellHighlightColor = [NSColor colorWithDeviceRed:0.549f green:0.561f blue:0.588f alpha:1];
        _cellEditingFillColor = [NSColor colorWithDeviceRed:0.141f green:0.141f blue:0.141f alpha:0.5f];
        _cellAlternatingRowColors = @[[NSColor colorWithCalibratedWhite:0.16f alpha:0.86f],
                                      [NSColor colorWithCalibratedWhite:0.15f alpha:0.8f]];
        _cellTextColor = [NSColor whiteColor];
        _cellSelectedTextColor = [NSColor blackColor];
        _strokeColor = [NSColor colorWithDeviceRed:0.749f green:0.761f blue:0.788f alpha:1.0f];
        _highlightGradient = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithDeviceRed:0.451f green:0.451f blue:0.455f alpha:0.5f]
                                                           endingColor:[NSColor colorWithDeviceRed:0.318f green:0.318f blue:0.318f alpha:0.5f]];
        _normalGradient = [[NSGradient alloc] initWithStartingColor:[NSColor colorWithDeviceRed:0.251f green:0.251f blue:0.255f alpha:0.5f]
                                                        endingColor:[NSColor colorWithDeviceRed:0.118f green:0.118f blue:0.118f alpha:0.5f]];


        [self setBackgroundColor:_tableBackgroundColor];
        [self setFocusRingType:NSFocusRingTypeNone];
    }

    return self;
}

- (id)_alternatingRowBackgroundColors {
    return _cellAlternatingRowColors;
}

- (id)_highlightColorForCell:(id)cell {
    return _cellHighlightColor;
}

- (void)_sendDelegateWillDisplayCell:(id)cell forColumn:(id)column row:(NSInteger)row {

    [super _sendDelegateWillDisplayCell:cell forColumn:column row:row];

    [[self currentEditor] setBackgroundColor:_cellEditingFillColor];
    [[self currentEditor] setTextColor:_cellTextColor];

    if([[self selectedRowIndexes] containsIndex: row]) {

        if([cell respondsToSelector: @selector(setTextColor:)]) {
            [cell setTextColor:_cellSelectedTextColor];
        }
    } else {

        if ([cell respondsToSelector:@selector(setTextColor:)]) {
            [cell setTextColor:_cellTextColor];
        }
    }
}

- (void)awakeFromNib {
    [self setCornerView: [[VLCHUDTableCornerView alloc] init]];
}

@end
