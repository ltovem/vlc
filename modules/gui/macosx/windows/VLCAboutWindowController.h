// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCAboutWindowController.h
 *****************************************************************************
 * Copyright (C) 2001-2013 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <thedj@users.sourceforge.net>
 *          Felix Paul Kühne <fkuehne -at- videolan.org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCAboutWindowController : NSWindowController<NSWindowDelegate>
{
    /* main about panel and stuff related to its views */
    IBOutlet id o_name_version_field;
    IBOutlet id o_revision_field;
    IBOutlet id o_copyright_field;
    IBOutlet id o_credits_textview;
    IBOutlet id o_credits_scrollview;
    IBOutlet id o_gpl_btn;
    IBOutlet id o_credits_btn;
    IBOutlet id o_authors_btn;
    IBOutlet id o_name_field;
    IBOutlet id o_icon_view;
    IBOutlet id o_joinus_txt;
    IBOutlet id o_trademarks_txt;
}

- (void)showAbout;
- (void)showGPL;
- (IBAction)buttonAction:(id)sender;

@end
