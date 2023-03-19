// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCTrackSynchronizationWindowController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2011-2012 VLC authors and VideoLAN
 * Copyright (C) 2011-2019 Felix Paul Kühne
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCTrackSynchronizationWindowController : NSWindowController

/* generic */
@property (readwrite, weak) IBOutlet NSButton *resetButton;

/* Audio / Video */
@property (readwrite, weak) IBOutlet NSTextField *avLabel;
@property (readwrite, weak) IBOutlet NSTextField *av_advanceLabel;
@property (readwrite, weak) IBOutlet NSTextField *av_advanceTextField;
@property (readwrite, weak) IBOutlet NSStepper *avStepper;

/* Subtitles / Video */
@property (readwrite, weak) IBOutlet NSTextField *svLabel;
@property (readwrite, weak) IBOutlet NSTextField *sv_advanceLabel;
@property (readwrite, weak) IBOutlet NSTextField *sv_advanceTextField;
@property (readwrite, weak) IBOutlet NSStepper *sv_advanceStepper;
@property (readwrite, weak) IBOutlet NSTextField *sv_speedLabel;
@property (readwrite, weak) IBOutlet NSTextField *sv_speedTextField;
@property (readwrite, weak) IBOutlet NSStepper *sv_speedStepper;
@property (readwrite, weak) IBOutlet NSTextField *sv_durLabel;
@property (readwrite, weak) IBOutlet NSTextField *sv_durTextField;
@property (readwrite, weak) IBOutlet NSStepper *sv_durStepper;

- (IBAction)toggleWindow:(id)sender;
- (IBAction)resetValues:(id)sender;

- (IBAction)avValueChanged:(id)sender;

- (IBAction)svAdvanceValueChanged:(id)sender;
- (IBAction)svSpeedValueChanged:(id)sender;
- (IBAction)svDurationValueChanged:(id)sender;
@end
