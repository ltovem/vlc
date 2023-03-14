/*****************************************************************************
 * VLCTextfieldPanelController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2012 Felix Paul Kühne
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCTextfieldPanelController : NSWindowController

@property (weak) IBOutlet NSTextField *titleLabel;
@property (weak) IBOutlet NSTextField *subtitleLabel;
@property (weak) IBOutlet NSTextField *textField;
@property (weak) IBOutlet NSButton *cancelButton;
@property (weak) IBOutlet NSButton *okButton;

@property (readwrite) NSString *titleString;
@property (readwrite) NSString *subTitleString;
@property (readwrite) NSString *okButtonString;
@property (readwrite) NSString *cancelButtonString;

/**
 * Completion handler for textfield panel
 * \param returnCode Result from panel. Can be NSModalResponseOK or NSModalResponseCancel.
 * \param resultingText Resulting text string entered in panel.
 */
typedef void(^TextfieldPanelCompletionBlock)(NSInteger returnCode, NSString *resultingText);

/**
 * Shows the panel as a modal dialog with window as its owner.
 * \param window Parent window for the dialog.
 * \param handler Completion block.
 */
- (void)runModalForWindow:(NSWindow *)window completionHandler:(TextfieldPanelCompletionBlock)handler;

- (IBAction)windowElementAction:(id)sender;

@end
