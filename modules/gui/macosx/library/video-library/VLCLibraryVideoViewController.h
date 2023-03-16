// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryVideoViewController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@class VLCLibraryWindow;
@class VLCLibraryVideoTableViewDataSource;
@class VLCLibraryVideoCollectionViewsStackViewController;

NS_ASSUME_NONNULL_BEGIN

// Controller for the video library views

@interface VLCLibraryVideoViewController : NSObject

@property (readonly) VLCLibraryWindow *libraryWindow;
@property (readonly) NSView *libraryTargetView;
@property (readonly) NSView *videoLibraryView;
@property (readonly) NSSplitView *videoLibrarySplitView;
@property (readonly) NSScrollView *videoLibraryCollectionViewsStackViewScrollView;
@property (readonly) NSStackView *videoLibraryCollectionViewsStackView;
@property (readonly) NSScrollView *videoLibraryGroupSelectionTableViewScrollView;
@property (readonly) NSTableView *videoLibraryGroupSelectionTableView;
@property (readonly) NSScrollView *videoLibraryGroupsTableViewScrollView;
@property (readonly) NSTableView *videoLibraryGroupsTableView;
@property (readonly) NSSegmentedControl *gridVsListSegmentedControl;
@property (readonly) NSSegmentedControl *segmentedTitleControl;
@property (readonly) NSImageView *placeholderImageView;
@property (readonly) NSTextField *placeholderLabel;
@property (readonly) NSView *emptyLibraryView;

@property (readonly) VLCLibraryVideoTableViewDataSource *libraryVideoTableViewDataSource;
@property (readonly) VLCLibraryVideoCollectionViewsStackViewController *libraryVideoCollectionViewsStackViewController;

@property (readonly) NSArray<NSLayoutConstraint *> *videoPlaceholderImageViewSizeConstraints;

- (instancetype)initWithLibraryWindow:(VLCLibraryWindow *)libraryWindow;
- (void)presentVideoView;


@end

NS_ASSUME_NONNULL_END
