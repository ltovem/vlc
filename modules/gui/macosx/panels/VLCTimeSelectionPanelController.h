// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * TimeSelectionPanelController.h: Controller for time selection panel
 *****************************************************************************
 * Copyright (C) 2015-2018 VideoLAN and authors
 * Author:       David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCTimeSelectionPanelController : NSWindowController

@property (readwrite, weak) IBOutlet NSButton *cancelButton;
@property (readwrite, weak) IBOutlet NSTextField *goToLabel;
@property (readwrite, weak) IBOutlet NSButton *okButton;
@property (readwrite, weak) IBOutlet NSTextField *secsLabel;
@property (readwrite, weak) IBOutlet NSTextField *minsLabel;
@property (readwrite, weak) IBOutlet NSTextField *hoursLabel;

@property (nonatomic) int jumpSecsValue;
@property (nonatomic) int jumpMinsValue;
@property (nonatomic) int jumpHoursValue;
@property (nonatomic) int secsMax;
@property (nonatomic) int minsMax;
@property (nonatomic) int hoursMax;

- (IBAction)buttonPressed:(id)sender;

- (int)getTimeInSecs;
- (void)setMaxTime:(int)secsMax;
- (void)setPosition:(int)secsPos;

/**
 * Completion handler for textfield panel
 * \param returnCode Result from panel. Can be NSModalResponseOK or NSModalResponseCancel.
 * \param returnTime Resulting time in seconds entered in panel.
 */
typedef void(^TimeSelectionCompletionHandler)(NSInteger returnCode, int64_t returnTime);

/**
 * Shows the panel as a modal dialog with window as its owner.
 * \param window Parent window for the dialog.
 * \param handler Completion block.
 */
- (void)runModalForWindow:(NSWindow *)window completionHandler:(TimeSelectionCompletionHandler)handler;

@end
