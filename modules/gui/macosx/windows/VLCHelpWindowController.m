/*****************************************************************************
 * HelpWindowController.m
 *****************************************************************************
 * Copyright (C) 2001-2014 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <thedj@users.sourceforge.net>
 *          Felix Paul Kühne <fkuehne -at- videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCHelpWindowController.h"

#import <vlc_intf_strings.h>
#import <vlc_about.h>

#import "extensions/NSString+Helpers.h"
#import "main/CompatibilityFixes.h"
#import "main/VLCMain.h"
#import "views/VLCScrollingClipView.h"

@implementation VLCHelpWindowController

- (id)init
{
    self = [super initWithWindowNibName:@"Help"];
    if (self) {
        [self setWindowFrameAutosaveName:@"help"];
    }

    return self;
}

- (void)windowDidLoad
{
    [[self window] setTitle:_NS("VLC media player Help")];
    [forwardButton setToolTip:_NS("Next")];
    [backButton setToolTip:_NS("Previous")];
    [homeButton setToolTip:_NS("Index")];
}

- (void)showHelp
{
    [self showWindow:nil];
    [self helpGoHome:nil];
}

- (IBAction)helpGoHome:(id)sender
{
    NSString *htmlWithStyle = [NSString
                               stringWithFormat:@"<style>body { font-family: -apple-system, Helvetica Neue; }</style>%@",
                               NSTR(I_LONGHELP)];

    [[helpWebView mainFrame] loadHTMLString:htmlWithStyle
                                    baseURL:[NSURL URLWithString:@"https://videolan.org"]];
}

- (void)webView:(WebView *)sender didFinishLoadForFrame:(WebFrame *)frame
{
    /* Update back/forward button states whenever a new page is loaded */
    [forwardButton setEnabled:[helpWebView canGoForward]];
    [backButton setEnabled:[helpWebView canGoBack]];
}

@end
