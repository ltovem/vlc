// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryWindowController.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import "VLCLibraryWindowController.h"

#import "library/VLCLibraryNavigationStack.h"
#import "library/VLCLibraryWindow.h"
#import "library/audio-library/VLCLibraryAudioViewController.h"
#import "main/VLCMain.h"

@implementation VLCLibraryWindowController

- (instancetype)initWithLibraryWindow
{
    self = [super initWithWindowNibName:@"VLCLibraryWindow"];
    return self;
}

- (void)windowDidLoad
{
    VLCLibraryWindow *window = (VLCLibraryWindow *)self.window;
    [window setRestorationClass:[self class]];
    [window setExcludedFromWindowsMenu:YES];
    [window setAcceptsMouseMovedEvents:YES];
    [window setContentMinSize:NSMakeSize(VLCLibraryWindowMinimalWidth, VLCLibraryWindowMinimalHeight)];

    // HACK: On initialisation, the window refuses to accept any border resizing. It seems the split view
    // holds a monopoly on the edges of the window (which can be seen as the right-side of the split view
    // lets you resize the playlist, and after doing so the window becomes resizeable.

    // This can be worked around by maximizing the window, or toggling the playlist.
    // Toggling the playlist is simplest.
    [window togglePlaylist];
    [window togglePlaylist];
}

+ (void)restoreWindowWithIdentifier:(NSUserInterfaceItemIdentifier)identifier
                              state:(NSCoder *)state
                  completionHandler:(void (^)(NSWindow *, NSError *))completionHandler
{
    if([identifier isEqualToString:VLCLibraryWindowIdentifier] == NO) {
        return;
    }

    if([VLCMain sharedInstance].libraryWindowController == nil) {
        [VLCMain sharedInstance].libraryWindowController = [[VLCLibraryWindowController alloc] initWithLibraryWindow];
    }

    VLCLibraryWindow *libraryWindow = [VLCMain sharedInstance].libraryWindow;

    NSInteger rememberedSelectedLibrarySegment = [state decodeIntegerForKey:@"macosx-library-selected-segment"];
    NSInteger rememberedSelectedLibraryViewModeSegment = [state decodeIntegerForKey:@"macosx-library-view-mode-selected-segment"];
    NSInteger rememberedSelectedLibraryViewAudioSegment = [state decodeIntegerForKey:@"macosx-library-audio-view-selected-segment"];

    [libraryWindow.segmentedTitleControl setSelectedSegment:rememberedSelectedLibrarySegment];
    [libraryWindow.gridVsListSegmentedControl setSelectedSegment:rememberedSelectedLibraryViewModeSegment];
    [libraryWindow.audioSegmentedControl setSelectedSegment:rememberedSelectedLibraryViewAudioSegment];

    [libraryWindow segmentedControlAction:self];
    if (rememberedSelectedLibrarySegment == VLCLibraryMusicSegment) {
        [libraryWindow.libraryAudioViewController segmentedControlAction:self];
    }

    completionHandler(libraryWindow, nil);
}

@end
