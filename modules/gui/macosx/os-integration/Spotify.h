/*****************************************************************************
 * Spotify.h
 *****************************************************************************
 * Copyright (C) 2014 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <AppKit/AppKit.h>
#import <ScriptingBridge/ScriptingBridge.h>

OSType const kSpotifyPlayerStateStopped = 'kPSS';
OSType const kSpotifyPlayerStatePlaying = 'kPSP';
OSType const kSpotifyPlayerStatePaused  = 'kPSp';

@interface SpotifyApplication : SBApplication
@property (readonly) OSType playerState;  // is Spotify stopped, paused, or playing?
- (void)play;
- (void)pause;
@end
