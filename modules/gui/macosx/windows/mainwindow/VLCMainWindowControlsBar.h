// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCMainWindowControlsBar.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2012-2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan dot org>
 *          David Fuhrmann <dfuhrmann # videolan dot org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "windows/mainwindow/VLCControlsBarCommon.h"

@class VLCVolumeSlider;

/*****************************************************************************
 * VLCMainWindowControlsBar
 *
 *  Holds all specific outlets, actions and code for the main window controls bar.
 *****************************************************************************/

@interface VLCMainWindowControlsBar : VLCControlsBarCommon

@property (readwrite, strong) IBOutlet NSButton *stopButton;

@property (readwrite, strong) IBOutlet NSButton *prevButton;
@property (readwrite, strong) IBOutlet NSButton *nextButton;

@property (readwrite, strong) IBOutlet NSButton *repeatButton;
@property (readwrite, strong) IBOutlet NSButton *shuffleButton;

@property (readwrite, strong) IBOutlet NSLayoutConstraint *artistNameTextFieldWidthConstraint;

@property (readwrite, strong) IBOutlet NSTextField *songArtistSeparatorTextField;
@property (readwrite, strong) IBOutlet NSButton *volumeUpButton;

- (IBAction)stop:(id)sender;

@end

