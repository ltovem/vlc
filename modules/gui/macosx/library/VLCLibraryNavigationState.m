/*****************************************************************************
 * VLCLibraryNavigationStack.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCLibraryNavigationState.h"

#import "VLCLibraryWindow.h"

#import "media-source/VLCLibraryMediaSourceViewController.h"
#import "media-source/VLCMediaSourceBaseDataSource.h"
#import "media-source/VLCMediaSourceDataSource.h"

@implementation VLCLibraryNavigationState

- (instancetype)initFromLibraryWindow:(VLCLibraryWindow *)libraryWindow
{
    VLCLibraryNavigationState* navState = [super init];

    if(navState) {
        _currentMediaSource = libraryWindow.libraryMediaSourceViewController.baseDataSource.childDataSource;
        _currentNodeDisplayed = libraryWindow.libraryMediaSourceViewController.baseDataSource.childDataSource.nodeToDisplay;
    }

    return navState;
}

@end
