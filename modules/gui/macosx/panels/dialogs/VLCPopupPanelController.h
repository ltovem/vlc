/*****************************************************************************
 * VLCPopupPanelController.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2012 Felix Paul Kühne
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCPopupPanelController : NSWindowController

@property (weak) IBOutlet NSTextField *titleLabel;
@property (weak) IBOutlet NSTextField *subtitleLabel;
@property (weak) IBOutlet NSPopUpButton *popupButton;
@property (weak) IBOutlet NSButton *cancelButton;
@property (weak) IBOutlet NSButton *okButton;

@property (readwrite) NSString *titleString;
@property (readwrite) NSString *subTitleString;
@property (readwrite) NSString *okButtonString;
@property (readwrite) NSString *cancelButtonString;
@property (readwrite) NSArray *popupButtonContent;

/**
 * Completion handler for popup panel
 * \param returnCode Result from panel. Can be NSModalResponseOK or NSModalResponseCancel.
 * \param selectedIndex Selected index of the popup in panel.
 */
typedef void(^PopupPanelCompletionBlock)(NSInteger returnCode, NSInteger selectedIndex);

/**
 * Shows the panel as a modal dialog with window as its owner.
 * \param window Parent window for the dialog.
 * \param handler Completion block.
 */
- (void)runModalForWindow:(NSWindow *)window completionHandler:(PopupPanelCompletionBlock)handler;

- (IBAction)windowElementAction:(id)sender;

@end
