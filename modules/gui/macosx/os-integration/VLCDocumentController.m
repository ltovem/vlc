/*****************************************************************************
 * VLCDocumentController.m
 ****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 * Authors: David Fuhrmann <david dot fuhrmann at googlemail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCDocumentController.h"

#import "main/VLCMain.h"

@implementation VLCDocumentController

+ (void)load
{
    /* For the custom document controller to be used by the app,
     * it needs to be created before the default NSDocumentController
     * is created.
     */
    (void)[[VLCDocumentController alloc] init];
}

- (IBAction)clearRecentDocuments:(id)sender
{
    msg_Dbg(getIntf(), "Clear recent items list and resume points");
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:[NSDictionary dictionary] forKey:@"recentlyPlayedMedia"];
    [defaults setObject:[NSArray array] forKey:@"recentlyPlayedMediaList"];

    [super clearRecentDocuments:sender];
}

@end
