/*****************************************************************************
 * HelpWindowController.h
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <thedj@users.sourceforge.net>
 *          Felix Paul Kühne <fkuehne -at- videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCErrorWindowController : NSWindowController

@property (readwrite, weak) IBOutlet NSButton    *cleanupButton;
@property (readwrite, weak) IBOutlet NSTableView *errorTable;

- (IBAction)cleanupTable:(id)sender;

- (void)addError:(NSString *)title withMsg:(NSString *)message;

@end
