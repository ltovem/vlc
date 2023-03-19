// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCTextfieldPanelController.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2012 Felix Paul Kühne
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *****************************************************************************/

#import "VLCTextfieldPanelController.h"

@interface VLCTextfieldPanelController()
{
    TextfieldPanelCompletionBlock _completionBlock;
}

@end

@implementation VLCTextfieldPanelController

- (id)init
{
    self = [super initWithWindowNibName:@"TextfieldPanel"];

    return self;
}

- (IBAction)windowElementAction:(id)sender
{
    [self.window orderOut:sender];
    [NSApp endSheet: self.window];

    if (_completionBlock)
        _completionBlock(sender == _okButton ? NSModalResponseOK : NSModalResponseCancel, [_textField stringValue]);
}

- (void)runModalForWindow:(NSWindow *)window completionHandler:(TextfieldPanelCompletionBlock)handler;
{
    [self window];

    [_titleLabel setStringValue:self.titleString];
    [_subtitleLabel setStringValue:self.subTitleString];
    [_cancelButton setTitle:self.cancelButtonString];
    [_okButton setTitle:self.okButtonString];
    [_textField setStringValue:@""];

    _completionBlock = [handler copy];

    [window beginSheet:self.window completionHandler:nil];
}

@end
