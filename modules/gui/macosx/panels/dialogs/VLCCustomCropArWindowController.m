// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCCustomCropArWindowController.m: Controller for custom crop / AR panel
 *****************************************************************************
 * Copyright (C) 2019 VideoLAN and authors
 * Author:       Felix Paul Kühne <fkuehne # videolan.org>
 *****************************************************************************/

#import "VLCCustomCropArWindowController.h"
#import "extensions/NSString+Helpers.h"

@interface VLCCustomCropArWindowController ()
{
    CustomCropArCompletionHandler _completionHandler;
}
@end

@implementation VLCCustomCropArWindowController

- (instancetype)init
{
    self = [super initWithWindowNibName:@"VLCCustomCropARPanel"];
    return self;
}

- (void)windowDidLoad {
    [super windowDidLoad];

    [_applyButton setTitle:_NS("Apply")];
    [_cancelButton setTitle:_NS("Cancel")];
    [_titleLabel setStringValue:self.title];
}

- (void)buttonPressed:(id)sender
{
    [self.window orderOut:sender];
    [NSApp endSheet: self.window];
    NSString *geometry = [NSString stringWithFormat:@"%@:%@", _numeratorTextField.stringValue, _denominatorTextField.stringValue];

    if (_completionHandler)
        _completionHandler(sender == _applyButton ? NSModalResponseOK : NSModalResponseCancel, geometry);
}

- (void)runModalForWindow:(NSWindow *)window completionHandler:(CustomCropArCompletionHandler)handler
{
    _completionHandler = handler;
    [window beginSheet:self.window completionHandler:nil];
}

@end
