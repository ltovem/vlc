// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLogWindowController.h: Log message window controller
 *****************************************************************************
 * Copyright (C) 2004-2013 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne at videolan dot org>
 *          Pierre d'Herbemont <pdherbemont # videolan org>
 *          Derk-Jan Hartman <hartman at videolan.org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCLogWindowController : NSWindowController

@property (assign) IBOutlet NSTableView *messageTable;
@property (assign) IBOutlet NSButton *saveButton;
@property (assign) IBOutlet NSButton *clearButton;
@property (assign) IBOutlet NSButton *refreshButton;
@property (assign) IBOutlet NSButton *toggleDetailsButton;
@property (assign) IBOutlet NSSplitView *splitView;
@property (assign) IBOutlet NSView *detailView;
@property (assign) IBOutlet NSArrayController *arrayController;

- (IBAction)saveDebugLog:(id)sender;
- (IBAction)refreshLog:(id)sender;
- (IBAction)clearLog:(id)sender;

@end
