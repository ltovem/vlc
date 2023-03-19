// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * TimeSelectionPanelController.m: Controller for time selection panel
 *****************************************************************************
 * Copyright (C) 2015-2018 VideoLAN and authors
 * Author:       David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *****************************************************************************/

#import "VLCTimeSelectionPanelController.h"

#import "extensions/NSString+Helpers.h"

@interface VLCTimeSelectionPanelController()
{
    TimeSelectionCompletionHandler _completionHandler;
}
@end

@implementation VLCTimeSelectionPanelController

#pragma mark - object handling

- (id)init
{
    self = [super initWithWindowNibName:@"TimeSelectionPanel"];
    return self;
}

#pragma mark - UI handling

- (void)windowDidLoad
{
    [_cancelButton setTitle: _NS("Cancel")];
    [_okButton setTitle: _NS("OK")];
    [_secsLabel setStringValue: _NS("ss")];
    [_minsLabel setStringValue: _NS("mm")];
    [_hoursLabel setStringValue: _NS("hh")];
    [_goToLabel setStringValue: _NS("Jump to Time")];
}

- (void)controlTextDidChange:(NSNotification *)notification
{
    [self setPosition:[self getTimeInSecs]];
}

- (void)setMaxTime:(int)secsMax
{
    [self setHoursMax:(int)secsMax / 3600];

    if (secsMax >= 3600) {
        [self setMinsMax:59];
        [self setSecsMax:59];
    }
    else if (secsMax >= 60) {
        [self setMinsMax:(int)secsMax / 60];
        [self setSecsMax:59];
    }
    else {
        [self setSecsMax:secsMax];
        [self setMinsMax:0];
    }
}

- (void)setPosition:(int)secsPos
{
    int minsPos = secsPos / 60;
    secsPos = secsPos % 60;
    int hoursPos = minsPos / 60;
    minsPos = minsPos % 60;

    [self setJumpSecsValue: secsPos];
    [self setJumpMinsValue: minsPos];
    [self setJumpHoursValue: hoursPos];
}

- (int)getTimeInSecs
{
    // calculate resulting time in secs:
    int timeInSec = self.jumpSecsValue;
    timeInSec += self.jumpMinsValue * 60;
    timeInSec += self.jumpHoursValue * 3600;
    return timeInSec;
}

- (IBAction)buttonPressed:(id)sender
{
    [self.window orderOut:sender];
    [NSApp endSheet: self.window];
    int64_t timeInSec = [self getTimeInSecs];

    if (_completionHandler)
        _completionHandler(sender == _okButton ? NSModalResponseOK : NSModalResponseCancel, timeInSec);
}

- (void)runModalForWindow:(NSWindow *)window completionHandler:(TimeSelectionCompletionHandler)handler
{
    _completionHandler = handler;
    [window beginSheet:self.window completionHandler:nil];
}

@end
