// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCTimeField.h: NSTextField subclass for playback time fields
 *****************************************************************************
 * Copyright (C) 2003-2017 VLC authors and VideoLAN
 *
 * Authors: Jon Lech Johansen <jon-vl@nanocrew.net>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *          Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

/*****************************************************************************
 * VLCTimeField interface
 *****************************************************************************
 * we need the implementation to catch our click-event in the controller window
 *****************************************************************************/

extern NSString *VLCTimeFieldDisplayTimeAsElapsed;
extern NSString *VLCTimeFieldDisplayTimeAsRemaining;

@interface VLCTimeField : NSTextField

@property (readwrite, assign, nonatomic) BOOL isTimeRemaining;

- (void)setRemainingIdentifier:(NSString *)identifier;
- (void)setTime:(NSString *)time withRemainingTime:(NSString *)remainingTime;

@end
