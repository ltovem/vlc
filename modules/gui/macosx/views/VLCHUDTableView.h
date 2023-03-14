//
//  VLCHUDTableView.h
//  BGHUDAppKit
//
//  Created by BinaryGod on 6/17/08.
//
//  Copyright (c) 2008, Tim Davis (BinaryMethod.com, binary.god@gmail.com)
//  All rights reserved.
//
//  SPDX-License-Identifier: BSD-3-Clause

#import <Cocoa/Cocoa.h>

@interface VLCHUDTableView : NSTableView

@property (strong) NSColor *tableBackgroundColor;
@property (strong) NSArray *cellAlternatingRowColors;
@property (strong) NSColor *cellHighlightColor;
@property (strong) NSColor *cellEditingFillColor;
@property (strong) NSColor *cellTextColor;
@property (strong) NSColor *cellSelectedTextColor;
@property (strong) NSColor *strokeColor;

@property (strong) NSGradient *highlightGradient;
@property (strong) NSGradient *normalGradient;

@end
