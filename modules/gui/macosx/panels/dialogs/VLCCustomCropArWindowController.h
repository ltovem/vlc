// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCCustomCropArWindowController.h: Controller for custom crop / AR panel
 *****************************************************************************
 * Copyright (C) 2019 VideoLAN and authors
 * Author:       Felix Paul Kühne <fkuehne # videolan.org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface VLCCustomCropArWindowController : NSWindowController

@property (readwrite, weak) IBOutlet NSButton *cancelButton;
@property (readwrite, weak) IBOutlet NSButton *applyButton;
@property (readwrite, weak) IBOutlet NSTextField *titleLabel;
@property (readwrite, weak) IBOutlet NSTextField *numeratorTextField;
@property (readwrite, weak) IBOutlet NSTextField *denominatorTextField;
@property (readwrite, retain) NSString *title;

- (IBAction)buttonPressed:(id)sender;

/**
 * \param returnCode Result from panel. Can be NSModalResponseOK or NSModalResponseCancel.
 * \param geometry Geometry based on numbers entered in panel
 */
typedef void(^CustomCropArCompletionHandler)(NSInteger returnCode, NSString *geometry);

/**
 * Shows the panel as a modal dialog with window as its owner.
 * \param window Parent window for the dialog.
 * \param handler Completion block.
 */
- (void)runModalForWindow:(NSWindow *)window completionHandler:(CustomCropArCompletionHandler)handler;

@end

NS_ASSUME_NONNULL_END
