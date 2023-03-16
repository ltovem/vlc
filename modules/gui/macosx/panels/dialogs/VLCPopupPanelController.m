// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCPopupPanelController.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2012 Felix Paul Kühne
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *****************************************************************************/

#import "VLCPopupPanelController.h"

@interface VLCPopupPanelController()
{
    PopupPanelCompletionBlock _completionBlock;
}
@end

@implementation VLCPopupPanelController

- (id)init
{
    self = [super initWithWindowNibName:@"PopupPanel"];

    return self;
}

- (IBAction)windowElementAction:(id)sender
{
    [self.window orderOut:sender];
    [NSApp endSheet: self.window];

    if (_completionBlock)
        _completionBlock(sender == _okButton ? NSModalResponseOK : NSModalResponseCancel, [_popupButton indexOfSelectedItem]);
}

- (void)runModalForWindow:(NSWindow *)window completionHandler:(PopupPanelCompletionBlock)handler;
{
    [self window];

    [_titleLabel setStringValue:self.titleString];
    [_subtitleLabel setStringValue:self.subTitleString];
    [_cancelButton setTitle:self.cancelButtonString];
    [_okButton setTitle:self.okButtonString];
    [_popupButton removeAllItems];
    for (NSString *value in self.popupButtonContent)
        [[_popupButton menu] addItemWithTitle:value action:nil keyEquivalent:@""];

    _completionBlock = [handler copy];

    [window beginSheet:self.window completionHandler:nil];
}

@end
