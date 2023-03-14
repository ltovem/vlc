/*****************************************************************************
 * VLCLibraryMediaSourceViewController.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <developer@claudiocambra.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCLibraryMediaSourceViewController.h"

#import "VLCMediaSourceBaseDataSource.h"

#import "library/VLCLibraryCollectionViewFlowLayout.h"
#import "library/VLCLibraryCollectionViewItem.h"
#import "library/VLCLibraryController.h"
#import "library/VLCLibraryUIUnits.h"
#import "library/VLCLibraryWindow.h"

#import "main/VLCMain.h"

@implementation VLCLibraryMediaSourceViewController

- (instancetype)initWithLibraryWindow:(VLCLibraryWindow *)libraryWindow
{
    self = [super init];
    if (self) {
        [self setupPropertiesFromLibraryWindow:libraryWindow];
        [self setupBaseDataSource];
        [self setupCollectionView];
        [self setupMediaSourceLibraryViews];
    }
    return self;
}

- (void)setupPropertiesFromLibraryWindow:(VLCLibraryWindow *)libraryWindow
{
    NSParameterAssert(libraryWindow);
    _libraryTargetView = libraryWindow.libraryTargetView;
    _mediaSourceView = libraryWindow.mediaSourceView;
    _mediaSourceTableView = libraryWindow.mediaSourceTableView;
    _collectionView = libraryWindow.mediaSourceCollectionView;
    _collectionViewScrollView = libraryWindow.mediaSourceCollectionViewScrollView;
    _tableView = libraryWindow.mediaSourceTableView;
    _tableViewScrollView = libraryWindow.mediaSourceTableViewScrollView;
    _homeButton = libraryWindow.mediaSourceHomeButton;
    _pathControl = libraryWindow.mediaSourcePathControl;
    _gridVsListSegmentedControl = libraryWindow.gridVsListSegmentedControl;
}

- (void)setupBaseDataSource
{
    _baseDataSource = [[VLCMediaSourceBaseDataSource alloc] init];
    _baseDataSource.collectionView = _collectionView;
    _baseDataSource.collectionViewScrollView = _collectionViewScrollView;
    _baseDataSource.homeButton = _homeButton;
    _baseDataSource.pathControl = _pathControl;
    _baseDataSource.gridVsListSegmentedControl = _gridVsListSegmentedControl;
    _baseDataSource.tableView = _tableView;
    [_baseDataSource setupViews];
}

- (void)setupCollectionView
{
    const CGFloat collectionItemSpacing = [VLCLibraryUIUnits collectionViewItemSpacing];
    const NSEdgeInsets collectionViewSectionInset = [VLCLibraryUIUnits collectionViewSectionInsets];

    NSCollectionViewFlowLayout *mediaSourceCollectionViewLayout = [[VLCLibraryCollectionViewFlowLayout alloc] init];
    _collectionView.collectionViewLayout = mediaSourceCollectionViewLayout;
    mediaSourceCollectionViewLayout.itemSize = [VLCLibraryCollectionViewItem defaultSize];
    mediaSourceCollectionViewLayout.minimumLineSpacing = collectionItemSpacing;
    mediaSourceCollectionViewLayout.minimumInteritemSpacing = collectionItemSpacing;
    mediaSourceCollectionViewLayout.sectionInset = collectionViewSectionInset;
}

- (void)setupMediaSourceLibraryViews
{
    _mediaSourceTableView.rowHeight = [VLCLibraryUIUnits mediumTableViewRowHeight];

    const NSEdgeInsets defaultInsets = [VLCLibraryUIUnits libraryViewScrollViewContentInsets];
    const NSEdgeInsets scrollerInsets = [VLCLibraryUIUnits libraryViewScrollViewScrollerInsets];

    _collectionViewScrollView.automaticallyAdjustsContentInsets = NO;
    _collectionViewScrollView.contentInsets = defaultInsets;
    _collectionViewScrollView.scrollerInsets = scrollerInsets;

    _tableViewScrollView.automaticallyAdjustsContentInsets = NO;
    _tableViewScrollView.contentInsets = defaultInsets;
    _tableViewScrollView.scrollerInsets = scrollerInsets;
}

- (void)presentBrowseView
{
    [self presentMediaSourceView:VLCLibraryBrowseSegment];
}

- (void)presentStreamsView
{
    [self presentMediaSourceView:VLCLibraryStreamsSegment];
}

- (void)presentMediaSourceView:(VLCLibrarySegment)viewSegment
{
    _libraryTargetView.subviews = @[];

    if (_mediaSourceView.superview == nil) {
        _mediaSourceView.translatesAutoresizingMaskIntoConstraints = NO;
        _libraryTargetView.subviews = @[_mediaSourceView];
        NSDictionary *dict = NSDictionaryOfVariableBindings(_mediaSourceView);
        [_libraryTargetView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_mediaSourceView(>=572.)]|" options:0 metrics:0 views:dict]];
        [_libraryTargetView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_mediaSourceView(>=444.)]|" options:0 metrics:0 views:dict]];
    }

    _baseDataSource.mediaSourceMode = viewSegment == VLCLibraryBrowseSegment ? VLCMediaSourceModeLAN : VLCMediaSourceModeInternet;
    [_baseDataSource reloadViews];
}

@end
