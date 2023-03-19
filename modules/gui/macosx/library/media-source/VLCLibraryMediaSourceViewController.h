// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryMediaSourceViewController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCInputNodePathControl;
@class VLCLibraryWindow;
@class VLCMediaSourceBaseDataSource;

@interface VLCLibraryMediaSourceViewController : NSObject

@property (readonly) NSView *libraryTargetView;
@property (readonly) NSView *mediaSourceView;
@property (readonly) NSTableView *mediaSourceTableView;
@property (readonly) NSCollectionView *collectionView;
@property (readonly) NSScrollView *collectionViewScrollView;
@property (readonly) NSTableView *tableView;
@property (readonly) NSScrollView *tableViewScrollView;
@property (readonly) NSButton *homeButton;
@property (readonly) VLCInputNodePathControl *pathControl;
@property (readonly) NSSegmentedControl *gridVsListSegmentedControl;

@property (readonly) VLCMediaSourceBaseDataSource *baseDataSource;

- (instancetype)initWithLibraryWindow:(VLCLibraryWindow *)libraryWindow;

- (void)presentBrowseView;
- (void)presentStreamsView;

@end

NS_ASSUME_NONNULL_END
