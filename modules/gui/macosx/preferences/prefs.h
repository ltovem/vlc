/*****************************************************************************
 * prefs.h: MacOS X module for vlc
 *****************************************************************************
 * Copyright (C) 2002-2015 VLC authors and VideoLAN
 *
 * Authors: Jon Lech Johansen <jon-vl@nanocrew.net>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@class VLCTreeMainItem;

/*****************************************************************************
 * VLCPrefs interface
 *****************************************************************************/
@interface VLCPrefs : NSWindowController<NSSplitViewDelegate>

@property (readwrite, weak) IBOutlet NSTextField *titleLabel;
@property (readwrite, weak) IBOutlet NSOutlineView *tree;
@property (readwrite, weak) IBOutlet NSScrollView *prefsView;
@property (readwrite, weak) IBOutlet NSButton *saveButton;
@property (readwrite, weak) IBOutlet NSButton *cancelButton;
@property (readwrite, weak) IBOutlet NSButton *resetButton;
@property (readwrite, weak) IBOutlet NSButton *showBasicButton;

- (void)setTitle: (NSString *) o_title_name;
- (void)showPrefsWithLevel:(NSInteger)i_window_level;
- (IBAction)savePrefs: (id)sender;
- (IBAction)closePrefs: (id)sender;
- (IBAction)showSimplePrefs: (id)sender;
- (IBAction)resetPrefs: (id)sender;

@end
