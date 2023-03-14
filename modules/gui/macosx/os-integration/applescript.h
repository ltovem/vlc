/*****************************************************************************
 * applescript.h: MacOS X AppleScript support
 *****************************************************************************
 * Copyright (C) 2002-2019 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <thedj@users.sourceforge.net>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

/*****************************************************************************
 * VLGetURLScriptCommand interface
 *****************************************************************************/
@interface VLGetURLScriptCommand : NSScriptCommand
@end

/*****************************************************************************
 * VLControlScriptCommand interface
 *****************************************************************************/
@interface VLControlScriptCommand : NSScriptCommand
@end

/*****************************************************************************
* Category that adds AppleScript support to NSApplication
*****************************************************************************/
@interface NSApplication(ScriptSupport)

@property (readwrite) BOOL scriptFullscreenMode;
@property (readwrite) float audioVolume;
@property (readwrite) long long audioDesync;
@property (readwrite) int currentTime;
@property (readwrite) float playbackRate;
@property (readonly) NSInteger durationOfCurrentItem;
@property (readonly) NSString *pathOfCurrentItem;
@property (readonly) NSString *nameOfCurrentItem;
@property (readonly) BOOL playbackShowsMenu;
@property (readonly) BOOL recordable;
@property (readwrite) BOOL recordingEnabled;
@property (readwrite) BOOL shuffledPlayback;
@property (readwrite) BOOL repeatOne;
@property (readwrite) BOOL repeatAll;

@end
