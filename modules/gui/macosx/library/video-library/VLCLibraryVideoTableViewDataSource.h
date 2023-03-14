/*****************************************************************************
 * VVLCLibraryVideoTableViewDataSource.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "library/VLCLibraryTableView.h"

NS_ASSUME_NONNULL_BEGIN

@class VLCLibraryModel;

@interface VLCLibraryVideoTableViewDataSource : NSObject <VLCLibraryTableViewDataSource, NSTableViewDelegate>

@property (readwrite, assign) VLCLibraryModel *libraryModel;
@property (readwrite, assign) NSTableView *groupsTableView;
@property (readwrite, assign) NSTableView *groupSelectionTableView;

- (void)setup;
- (void)reloadData;

@end

NS_ASSUME_NONNULL_END
