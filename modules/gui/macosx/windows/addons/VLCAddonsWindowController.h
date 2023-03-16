// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCAddonsWindowController.h: Addons manager for the Mac
 ****************************************************************************
 * Copyright (C) 2014 VideoLAN and authors
 * Author:       Felix Paul Kühne <fkuehne # videolan.org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCAddonsWindowController : NSWindowController
{
    IBOutlet NSPopUpButton *_typeSwitcher;
    IBOutlet NSButton *_localAddonsOnlyCheckbox;
    IBOutlet NSButton *_downloadCatalogButton;

    IBOutlet NSTableView *_addonsTable;
    IBOutlet NSProgressIndicator *_spinner;
    IBOutlet NSTextField *_name;
    IBOutlet NSTextField *_author;
    IBOutlet NSTextField *_version;
    IBOutlet NSTextView *_description;
    IBOutlet NSButton *_installButton;
}

- (IBAction)switchType:(id)sender;
- (IBAction)toggleLocalCheckbox:(id)sender;
- (IBAction)downloadCatalog:(id)sender;
- (IBAction)installSelection:(id)sender;
- (IBAction)uninstallSelection:(id)sender;

@end