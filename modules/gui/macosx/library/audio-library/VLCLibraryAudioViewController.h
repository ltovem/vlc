// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryAudioViewController.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

@class VLCLibraryAudioDataSource;
@class VLCLibraryAudioGroupDataSource;
@class VLCLibraryWindow;

NS_ASSUME_NONNULL_BEGIN

@interface VLCLibraryAudioViewController : NSObject

@property (readonly) VLCLibraryWindow *libraryWindow;
@property (readonly) NSView *libraryTargetView;
@property (readonly) NSView *audioLibraryView;
@property (readonly) NSSplitView *audioLibrarySplitView;
@property (readonly) NSScrollView *audioCollectionSelectionTableViewScrollView;
@property (readonly) NSTableView *audioCollectionSelectionTableView;
@property (readonly) NSScrollView *audioGroupSelectionTableViewScrollView;
@property (readonly) NSTableView *audioGroupSelectionTableView;
@property (readonly) NSScrollView *audioSongTableViewScrollView;
@property (readonly) NSTableView *audioSongTableView;
@property (readonly) NSScrollView *audioCollectionViewScrollView;
@property (readonly) NSCollectionView *audioLibraryCollectionView;
@property (readonly) NSSplitView *audioLibraryGridModeSplitView;
@property (readonly) NSScrollView *audioLibraryGridModeSplitViewListTableViewScrollView;
@property (readonly) NSTableView *audioLibraryGridModeSplitViewListTableView;
@property (readonly) NSScrollView *audioLibraryGridModeSplitViewListSelectionCollectionViewScrollView;
@property (readonly) NSCollectionView *audioLibraryGridModeSplitViewListSelectionCollectionView;
@property (readonly) NSSegmentedControl *audioSegmentedControl;
@property (readonly) NSSegmentedControl *gridVsListSegmentedControl;
@property (readonly) NSSegmentedControl *segmentedTitleControl;
@property (readonly) NSImageView *placeholderImageView;
@property (readonly) NSTextField *placeholderLabel;
@property (readonly) NSView *emptyLibraryView;
@property (readonly) NSVisualEffectView *optionBarView;

@property (readonly) VLCLibraryAudioDataSource *audioDataSource;
@property (readonly) VLCLibraryAudioGroupDataSource *audioGroupDataSource;

@property (readonly) NSArray<NSLayoutConstraint *> *audioPlaceholderImageViewSizeConstraints;

- (instancetype)initWithLibraryWindow:(VLCLibraryWindow *)libraryWindow;
- (IBAction)segmentedControlAction:(id)sender;
- (void)presentAudioView;
- (void)reloadData;

@end

NS_ASSUME_NONNULL_END
