// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * HelpWindowController.h
 *****************************************************************************
 * Copyright (C) 2001-2013 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <thedj@users.sourceforge.net>
 *          Felix Paul Kühne <fkuehne -at- videolan.org>
 *****************************************************************************/

#import <WebKit/WebKit.h>

@interface VLCHelpWindowController : NSWindowController
{
    IBOutlet WebView *helpWebView;
    IBOutlet id backButton;
    IBOutlet id forwardButton;
    IBOutlet id homeButton;
}

- (IBAction)helpGoHome:(id)sender;
- (void)showHelp;

@end
