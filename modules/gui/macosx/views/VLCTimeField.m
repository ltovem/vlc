/*****************************************************************************
 * VLCTimeField.m: NSTextField subclass for playback time fields
 *****************************************************************************
 * Copyright (C) 2003-2017 VLC authors and VideoLAN
 *
 * Authors: Jon Lech Johansen <jon-vl@nanocrew.net>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *          Marvin Scholz <epirat07 at gmail dot com>
 *          Claudio Cambra <developer at claudiocambra dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCTimeField.h"

#import "main/VLCMain.h"
#import "menus/VLCMainMenu.h"

NSString *VLCTimeFieldDisplayTimeAsElapsed = @"DisplayTimeAsTimeElapsed";
NSString *VLCTimeFieldDisplayTimeAsRemaining = @"DisplayTimeAsTimeRemaining";

@interface VLCTimeField ()
{
    NSString *_identifier;
    NSString *_cachedTime;
    NSString *_remainingTime;
}
@end

@implementation VLCTimeField

+ (void)initialize
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary *appDefaults = [NSDictionary dictionaryWithObjectsAndKeys:
                                 @"NO", VLCTimeFieldDisplayTimeAsRemaining,
                                 nil];

    [defaults registerDefaults:appDefaults];
}


- (void)setRemainingIdentifier:(NSString *)identifier
{
    _identifier = identifier;
    _isTimeRemaining = [[NSUserDefaults standardUserDefaults] boolForKey:_identifier];
}

- (void)setIsTimeRemaining:(BOOL)isTimeRemaining
{
    _isTimeRemaining = isTimeRemaining;

    if (_identifier) {
        [[NSUserDefaults standardUserDefaults] setObject:(_isTimeRemaining ? @"YES" : @"NO") forKey:_identifier];
    }

    [self updateTimeValue];
}

- (void)mouseDown: (NSEvent *)ourEvent
{
    if ( [ourEvent clickCount] > 1 ) {
        [[[VLCMain sharedInstance] mainMenu] goToSpecificTime: nil];
    } else {
        self.isTimeRemaining = !self.isTimeRemaining;
    }

    [self updateTimeValue];
    [[self nextResponder] mouseDown:ourEvent];
}

- (void)setTime:(NSString *)time withRemainingTime:(NSString *)remainingTime
{
    _cachedTime = time;
    _remainingTime = remainingTime;

    [self updateTimeValue];
}

- (void)updateTimeValue
{
    if (!_cachedTime || !_remainingTime)
        return;

    if ([self timeRemaining]) {
        [super setStringValue:_remainingTime];
    } else {
        [super setStringValue:_cachedTime];
    }
}

- (void)setStringValue:(NSString *)stringValue
{
    [super setStringValue:stringValue];

    _cachedTime = nil;
    _remainingTime = nil;
}

- (BOOL)timeRemaining
{
    if (_identifier)
        return [[NSUserDefaults standardUserDefaults] boolForKey:_identifier];
    else
        return _isTimeRemaining;
}

@end
