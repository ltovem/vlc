// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCMainVideoViewController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import <windows/mainwindow/VLCControlsBarCommon.h>
#import <windows/video/VLCVoutView.h>

@class VLCMainVideoViewOverlayView;

NS_ASSUME_NONNULL_BEGIN

@interface VLCMainVideoViewController : NSViewController

@property (readwrite, strong) IBOutlet VLCVoutView *voutView;
@property (readwrite, strong) IBOutlet NSBox *mainControlsView;
@property (readwrite, strong) IBOutlet VLCMainVideoViewOverlayView *overlayView;
@property (readwrite, strong) IBOutlet NSView *bottomBarView;
@property (readwrite, strong) IBOutlet NSStackView *centralControlsStackView;
@property (readwrite, strong) IBOutlet VLCControlsBarCommon *controlsBar;
@property (readwrite, strong) IBOutlet NSButton *returnButton;
@property (readwrite, strong) IBOutlet NSButton *playlistButton;
@property (readwrite, strong) IBOutlet NSLayoutConstraint *returnButtonTopConstraint;
@property (readwrite, strong) IBOutlet NSLayoutConstraint *returnButtonLeadingConstraint;
@property (readwrite, strong) IBOutlet NSLayoutConstraint *playlistButtonTopConstraint;
@property (readwrite, strong) IBOutlet NSLayoutConstraint *playlistButtonTrailingConstraint;
@property (readwrite, strong) IBOutlet NSVisualEffectView *fakeTitleBar;
@property (readwrite, strong) IBOutlet NSLayoutConstraint *fakeTitleBarHeightConstraint;

@property (readwrite, nonatomic) BOOL autohideControls;
@property (readwrite, nonatomic) BOOL displayLibraryControls;
@property (readonly) BOOL mouseOnControls;

- (void)showControls;
- (void)hideControls;

- (IBAction)togglePlaylist:(id)sender;
- (IBAction)returnToLibrary:(id)sender;

@end

NS_ASSUME_NONNULL_END
