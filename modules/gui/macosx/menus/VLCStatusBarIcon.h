/*****************************************************************************
 * VLCStatusBarIcon.h: Status bar icon controller/delegate
 *****************************************************************************
 * Copyright (C) 2016-2019 VLC authors and VideoLAN
 *
 * Authors: Goran Dokic <vlc at 8hz dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@interface VLCStatusBarIcon : NSObject <NSMenuDelegate>

@property (readwrite, strong) NSStatusItem *statusItem;
@property (readwrite, strong) IBOutlet NSMenu *vlcStatusBarIconMenu;

@property (strong) IBOutlet NSView *playbackInfoView;
@property (strong) IBOutlet NSView *controlsView;

- (IBAction)statusBarIconShowMainWindow:(id)sender;
- (IBAction)statusBarIconTogglePlayPause:(id)sender;
- (IBAction)statusBarIconNext:(id)sender;
- (IBAction)statusBarIconPrevious:(id)sender;
- (IBAction)statusBarIconToggleRandom:(id)sender;
- (IBAction)statusBarIconShowMiniAudioPlayer:(id)sender;

@end
