/*****************************************************************************
 * VLCRemoteControlService.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2017, 2018 VLC authors and VideoLAN
 *
 * Authors: Carola Nitz <nitz.carola # gmail.com>
 *          Felix Paul Kühne <fkuehne # videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCRemoteControlService : NSObject

- (void)subscribeToRemoteCommands;
- (void)unsubscribeFromRemoteCommands;

@end
