/*****************************************************************************
 * VLCLibraryAudioViewController.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#import "VLCLibraryAudioViewController.h"

#import "extensions/NSString+Helpers.h"
#import "extensions/NSWindow+VLCAdditions.h"

#import "library/VLCLibraryCollectionViewDelegate.h"
#import "library/VLCLibraryCollectionViewFlowLayout.h"
#import "library/VLCLibraryController.h"
#import "library/VLCLibraryDataTypes.h"
#import "library/VLCLibraryModel.h"
#import "library/VLCLibraryNavigationStack.h"
#import "library/VLCLibrarySegment.h"
#import "library/VLCLibraryTwoPaneSplitViewDelegate.h"
#import "library/VLCLibraryWindow.h"
#import "library/VLCLibraryWindowPersistentPreferences.h"

#import "library/audio-library/VLCLibraryAlbumTableCellView.h"
#import "library/audio-library/VLCLibraryAudioDataSource.h"
#import "library/audio-library/VLCLibraryAudioGroupDataSource.h"
#import "library/audio-library/VLCLibraryAudioGroupHeaderView.h"
#import "library/audio-library/VLCLibraryAudioGroupTableViewDelegate.h"
#import "library/audio-library/VLCLibraryAudioTableViewDelegate.h"

#import "library/playlist-library/VLCLibraryPlaylistViewController.h"

#import "library/video-library/VLCLibraryVideoViewController.h"

#import "main/VLCMain.h"

#import "windows/video/VLCVoutView.h"
#import "windows/video/VLCMainVideoViewController.h"

NSString *VLCLibraryPlaceholderAudioViewIdentifier = @"VLCLibraryPlaceholderAudioViewIdentifier";

@interface VLCLibraryAudioViewController()
{
    id<VLCMediaLibraryItemProtocol> _awaitingPresentingLibraryItem;

    NSArray<NSString *> *_placeholderImageNames;
    NSArray<NSString *> *_placeholderLabelStrings;

    VLCLibraryCollectionViewDelegate *_audioLibraryCollectionViewDelegate;
    VLCLibraryAudioTableViewDelegate *_audioLibraryTableViewDelegate;
    VLCLibraryAudioGroupTableViewDelegate *_audioGroupLibraryTableViewDelegate;
    VLCLibraryTwoPaneSplitViewDelegate *_splitViewDelegate;
}
@end

@implementation VLCLibraryAudioViewController

#pragma mark - Set up the view controller

- (instancetype)initWithLibraryWindow:(VLCLibraryWindow *)libraryWindow
{
    self = [super init];

    if (self) {
        _currentSegmentType = VLCLibraryLowSentinelSegment;

        [self setupPropertiesFromLibraryWindow:libraryWindow];
        [self setupAudioDataSource];

        _audioLibraryCollectionViewDelegate = [[VLCLibraryCollectionViewDelegate alloc] init];
        _audioLibraryTableViewDelegate = [[VLCLibraryAudioTableViewDelegate alloc] init];
        _audioGroupLibraryTableViewDelegate = [[VLCLibraryAudioGroupTableViewDelegate alloc] init];
        _splitViewDelegate = [[VLCLibraryTwoPaneSplitViewDelegate alloc] init];

        [self setupAudioPlaceholderView];
        [self setupAudioCollectionView];
        [self setupGridModeSplitView];
        [self setupAudioTableViews];
        [self setupAudioLibraryViews];

        NSNotificationCenter *notificationCenter = NSNotificationCenter.defaultCenter;
        [notificationCenter addObserver:self
                               selector:@selector(libraryModelUpdated:)
                                   name:VLCLibraryModelAudioMediaListReset
                                 object:nil];
        [notificationCenter addObserver:self
                               selector:@selector(libraryModelUpdated:)
                                   name:VLCLibraryModelAudioMediaItemDeleted
                                 object:nil];
    }

    return self;
}

- (void)setupPropertiesFromLibraryWindow:(VLCLibraryWindow*)libraryWindow
{
    NSParameterAssert(libraryWindow);

    _libraryWindow = libraryWindow;
    _libraryTargetView = libraryWindow.libraryTargetView;
    _audioLibraryView = libraryWindow.audioLibraryView;
    _audioLibrarySplitView = libraryWindow.audioLibrarySplitView;
    _audioCollectionSelectionTableViewScrollView = libraryWindow.audioCollectionSelectionTableViewScrollView;
    _audioCollectionSelectionTableView = libraryWindow.audioCollectionSelectionTableView;
    _audioGroupSelectionTableViewScrollView = libraryWindow.audioGroupSelectionTableViewScrollView;
    _audioGroupSelectionTableView = libraryWindow.audioGroupSelectionTableView;
    _audioSongTableViewScrollView = libraryWindow.audioLibrarySongsTableViewScrollView;
    _audioSongTableView = libraryWindow.audioLibrarySongsTableView;
    _audioCollectionViewScrollView = libraryWindow.audioCollectionViewScrollView;
    _audioLibraryCollectionView = libraryWindow.audioLibraryCollectionView;
    _audioLibraryGridModeSplitView = libraryWindow.audioLibraryGridModeSplitView;
    _audioLibraryGridModeSplitViewListTableViewScrollView = libraryWindow.audioLibraryGridModeSplitViewListTableViewScrollView;
    _audioLibraryGridModeSplitViewListTableView = libraryWindow.audioLibraryGridModeSplitViewListTableView;
    _audioLibraryGridModeSplitViewListSelectionCollectionViewScrollView = libraryWindow.audioLibraryGridModeSplitViewListSelectionCollectionViewScrollView;
    _audioLibraryGridModeSplitViewListSelectionCollectionView = libraryWindow.audioLibraryGridModeSplitViewListSelectionCollectionView;

    _placeholderImageView = libraryWindow.placeholderImageView;
    _placeholderLabel = libraryWindow.placeholderLabel;
    _emptyLibraryView = libraryWindow.emptyLibraryView;
}

- (void)setupAudioDataSource
{
    _audioDataSource = [[VLCLibraryAudioDataSource alloc] init];
    _audioDataSource.libraryModel = VLCMain.sharedInstance.libraryController.libraryModel;
    _audioDataSource.collectionSelectionTableView = _audioCollectionSelectionTableView;
    _audioDataSource.songsTableView = _audioSongTableView;
    _audioDataSource.collectionView = _audioLibraryCollectionView;
    _audioDataSource.gridModeListTableView = _audioLibraryGridModeSplitViewListTableView;
    [_audioDataSource setup];

    _audioGroupDataSource = [[VLCLibraryAudioGroupDataSource alloc] init];
    _audioGroupDataSource.tableViews = @[_audioGroupSelectionTableView];
    _audioGroupDataSource.collectionViews = @[_audioLibraryGridModeSplitViewListSelectionCollectionView];
    _audioDataSource.audioGroupDataSource = _audioGroupDataSource;
}

- (void)setupAudioCollectionView
{
    _audioLibraryCollectionView.dataSource = _audioDataSource;
    _audioLibraryCollectionView.delegate = _audioLibraryCollectionViewDelegate;

    _audioLibraryCollectionView.selectable = YES;
    _audioLibraryCollectionView.allowsMultipleSelection = NO;
    _audioLibraryCollectionView.allowsEmptySelection = YES;
    _audioLibraryCollectionView.collectionViewLayout = VLCLibraryCollectionViewFlowLayout.standardLayout;
}

- (void)setupAudioTableViews
{
    _audioLibrarySplitView.delegate = _splitViewDelegate;

    _audioCollectionSelectionTableView.dataSource = _audioDataSource;
    _audioCollectionSelectionTableView.delegate = _audioLibraryTableViewDelegate;

    _audioGroupSelectionTableView.dataSource = _audioGroupDataSource;
    _audioGroupSelectionTableView.delegate = _audioGroupLibraryTableViewDelegate;

    if(@available(macOS 11.0, *)) {
        _audioGroupSelectionTableView.style = NSTableViewStyleFullWidth;
    }

    _audioSongTableView.dataSource = _audioDataSource;
    _audioSongTableView.delegate = _audioLibraryTableViewDelegate;
}

- (void)setupGridModeSplitView
{
    _audioLibraryGridModeSplitView.delegate = _splitViewDelegate;

    _audioLibraryGridModeSplitViewListTableView.dataSource = _audioDataSource;
    _audioLibraryGridModeSplitViewListTableView.delegate = _audioLibraryTableViewDelegate;

    _audioLibraryGridModeSplitViewListSelectionCollectionView.dataSource = _audioGroupDataSource;
    _audioLibraryGridModeSplitViewListSelectionCollectionView.delegate = _audioLibraryCollectionViewDelegate;

    _audioLibraryGridModeSplitViewListSelectionCollectionView.selectable = YES;
    _audioLibraryGridModeSplitViewListSelectionCollectionView.allowsMultipleSelection = NO;
    _audioLibraryGridModeSplitViewListSelectionCollectionView.allowsEmptySelection = YES;

    VLCLibraryCollectionViewFlowLayout * const audioLibraryGridModeListSelectionCollectionViewLayout = VLCLibraryCollectionViewFlowLayout.standardLayout;
    _audioLibraryGridModeSplitViewListSelectionCollectionView.collectionViewLayout = audioLibraryGridModeListSelectionCollectionViewLayout;
    audioLibraryGridModeListSelectionCollectionViewLayout.headerReferenceSize = VLCLibraryAudioGroupHeaderView.defaultHeaderSize;

    if (@available(macOS 10.12, *)) {
        audioLibraryGridModeListSelectionCollectionViewLayout.sectionHeadersPinToVisibleBounds = YES;
    }

    [VLCLibraryAudioDataSource setupCollectionView:_audioLibraryGridModeSplitViewListSelectionCollectionView];
    [VLCLibraryAudioGroupDataSource setupCollectionView:_audioLibraryGridModeSplitViewListSelectionCollectionView];
}

- (void)setupAudioPlaceholderView
{
    _audioPlaceholderImageViewSizeConstraints = @[
        [NSLayoutConstraint constraintWithItem:_placeholderImageView
                                     attribute:NSLayoutAttributeWidth
                                     relatedBy:NSLayoutRelationEqual
                                        toItem:nil
                                     attribute:NSLayoutAttributeNotAnAttribute
                                    multiplier:0.f
                                      constant:149.f],
        [NSLayoutConstraint constraintWithItem:_placeholderImageView
                                     attribute:NSLayoutAttributeHeight
                                     relatedBy:NSLayoutRelationEqual
                                        toItem:nil
                                     attribute:NSLayoutAttributeNotAnAttribute
                                    multiplier:0.f
                                      constant:149.f],
    ];

    _placeholderImageNames = @[@"placeholder-group2", @"placeholder-music", @"placeholder-music", @"placeholder-music"];
    _placeholderLabelStrings = @[
        _NS("Your favorite artists will appear here.\nGo to the Browse section to add artists you love."),
        _NS("Your favorite albums will appear here.\nGo to the Browse section to add albums you love."),
        _NS("Your favorite tracks will appear here.\nGo to the Browse section to add tracks you love."),
        _NS("Your favorite genres will appear here.\nGo to the Browse section to add genres you love."),
    ];
}

- (void)setupAudioLibraryViews
{
    _audioCollectionSelectionTableView.rowHeight = VLCLibraryUIUnits.mediumTableViewRowHeight;
    _audioLibraryGridModeSplitViewListTableView.rowHeight = VLCLibraryUIUnits.mediumTableViewRowHeight;
    _audioGroupSelectionTableView.rowHeight = VLCLibraryAlbumTableCellView.defaultHeight;

    const NSEdgeInsets audioScrollViewContentInsets = VLCLibraryUIUnits.libraryViewScrollViewContentInsets;
    const NSEdgeInsets audioScrollViewScrollerInsets = VLCLibraryUIUnits.libraryViewScrollViewScrollerInsets;

    _audioCollectionViewScrollView.automaticallyAdjustsContentInsets = NO;
    _audioCollectionViewScrollView.contentInsets = audioScrollViewContentInsets;
    _audioCollectionViewScrollView.scrollerInsets = audioScrollViewScrollerInsets;

    _audioCollectionSelectionTableViewScrollView.automaticallyAdjustsContentInsets = NO;
    _audioCollectionSelectionTableViewScrollView.contentInsets = audioScrollViewContentInsets;
    _audioCollectionSelectionTableViewScrollView.scrollerInsets = audioScrollViewScrollerInsets;
    _audioGroupSelectionTableViewScrollView.automaticallyAdjustsContentInsets = NO;
    _audioGroupSelectionTableViewScrollView.contentInsets = audioScrollViewContentInsets;
    _audioGroupSelectionTableViewScrollView.scrollerInsets = audioScrollViewScrollerInsets;

    _audioLibraryGridModeSplitViewListTableViewScrollView.automaticallyAdjustsContentInsets = NO;
    _audioLibraryGridModeSplitViewListTableViewScrollView.contentInsets = audioScrollViewContentInsets;
    _audioLibraryGridModeSplitViewListTableViewScrollView.scrollerInsets = audioScrollViewScrollerInsets;
    _audioLibraryGridModeSplitViewListSelectionCollectionViewScrollView.automaticallyAdjustsContentInsets = NO;
    _audioLibraryGridModeSplitViewListSelectionCollectionViewScrollView.contentInsets = audioScrollViewContentInsets;
    _audioLibraryGridModeSplitViewListSelectionCollectionViewScrollView.scrollerInsets = audioScrollViewScrollerInsets;
}

#pragma mark - Show the audio view

- (void)presentAudioView
{
    _libraryTargetView.subviews = @[];
    [self updatePresentedView];
}

- (void)presentPlaceholderAudioView
{
    for (NSLayoutConstraint * const constraint in _libraryWindow.libraryVideoViewController.videoPlaceholderImageViewSizeConstraints) {
        constraint.active = NO;
    }
    for (NSLayoutConstraint * const constraint in _libraryWindow.libraryPlaylistViewController.placeholderImageViewConstraints) {
        constraint.active = NO;
    }
    for (NSLayoutConstraint * const constraint in _audioPlaceholderImageViewSizeConstraints) {
        constraint.active = YES;
    }

    const NSInteger selectedLibrarySegment = self.audioDataSource.audioLibrarySegment;
    NSAssert(selectedLibrarySegment != VLCAudioLibraryRecentsSegment &&
             selectedLibrarySegment != VLCAudioLibraryUnknownSegment,
             @"Received invalid audio library segment from audio data source!");

    if(selectedLibrarySegment < _placeholderImageNames.count && selectedLibrarySegment >= 0) {
        _placeholderImageView.image = [NSImage imageNamed:_placeholderImageNames[selectedLibrarySegment]];
    }

    if(selectedLibrarySegment < _placeholderLabelStrings.count && selectedLibrarySegment >= 0) {
        _placeholderLabel.stringValue = _placeholderLabelStrings[selectedLibrarySegment];
    }

    _emptyLibraryView.translatesAutoresizingMaskIntoConstraints = NO;
    _libraryTargetView.subviews = @[_emptyLibraryView];
    NSDictionary * const dict = NSDictionaryOfVariableBindings(_emptyLibraryView);
    [_libraryTargetView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_emptyLibraryView(>=572.)]|" options:0 metrics:0 views:dict]];
    [_libraryTargetView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_emptyLibraryView(>=444.)]|" options:0 metrics:0 views:dict]];

    _emptyLibraryView.identifier = VLCLibraryPlaceholderAudioViewIdentifier;
}

- (void)prepareAudioLibraryView
{
    _audioLibraryView.translatesAutoresizingMaskIntoConstraints = NO;
    _libraryTargetView.subviews = @[_audioLibraryView];
    NSDictionary *dict = NSDictionaryOfVariableBindings(_audioLibraryView);
    [_libraryTargetView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_audioLibraryView(>=572.)]|" options:0 metrics:0 views:dict]];
    [_libraryTargetView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_audioLibraryView(>=444.)]|" options:0 metrics:0 views:dict]];
}

- (void)hideAllViews
{
    _audioLibrarySplitView.hidden = YES;
    _audioLibraryGridModeSplitView.hidden = YES;
    _audioSongTableViewScrollView.hidden = YES;
    _audioCollectionViewScrollView.hidden = YES;
}

- (void)presentAudioGridModeView
{
    const VLCLibrarySegmentType selectedSegment = self.currentSegmentType;
    if (selectedSegment == VLCLibrarySongsMusicSubSegment ||
        selectedSegment == VLCLibraryAlbumsMusicSubSegment) {

        [_audioLibraryCollectionView deselectAll:self];
        [(VLCLibraryCollectionViewFlowLayout *)_audioLibraryCollectionView.collectionViewLayout resetLayout];

        _audioCollectionViewScrollView.hidden = NO;
    } else {
        _audioLibraryGridModeSplitView.hidden = NO;
    }
}

- (void)presentAudioTableView
{
    if (self.currentSegmentType == VLCLibrarySongsMusicSubSegment) {
        _audioSongTableViewScrollView.hidden = NO;
    } else {
        _audioLibrarySplitView.hidden = NO;
    }
}

- (VLCLibraryViewModeSegment)viewModeSegmentForCurrentLibrarySegment
{
    VLCLibraryWindowPersistentPreferences * const libraryWindowPrefs = VLCLibraryWindowPersistentPreferences.sharedInstance;

    switch (self.currentSegmentType) {
        case VLCLibraryArtistsMusicSubSegment:
            return libraryWindowPrefs.artistLibraryViewMode;
        case VLCLibraryGenresMusicSubSegment:
            return libraryWindowPrefs.genreLibraryViewMode;
        case VLCLibrarySongsMusicSubSegment:
            return libraryWindowPrefs.songsLibraryViewMode;
        case VLCLibraryAlbumsMusicSubSegment:
            return libraryWindowPrefs.albumLibraryViewMode;
        default:
            return VLCLibraryGridViewModeSegment;
    }
}

- (VLCAudioLibrarySegment)currentLibrarySegmentToAudioLibrarySegment
{
    switch (self.currentSegmentType) {
        case VLCLibraryMusicSegment:
        case VLCLibraryArtistsMusicSubSegment:
            return VLCAudioLibraryArtistsSegment;
        case VLCLibraryAlbumsMusicSubSegment:
            return VLCAudioLibraryAlbumsSegment;
        case VLCLibrarySongsMusicSubSegment:
            return VLCAudioLibrarySongsSegment;
        case VLCLibraryGenresMusicSubSegment:
            return VLCAudioLibraryGenresSegment;
        default:
            NSAssert(false, @"Non-audio or unknown library subsegment received");
            return VLCAudioLibraryUnknownSegment;
    }
}

- (void)updatePresentedView
{
    if (_audioDataSource.libraryModel.numberOfAudioMedia == 0) {
        [self presentPlaceholderAudioView];
    } else {
        [self prepareAudioLibraryView];
        [self hideAllViews];

        _audioDataSource.audioLibrarySegment = [self currentLibrarySegmentToAudioLibrarySegment];
        const VLCLibraryViewModeSegment viewModeSegment = [self viewModeSegmentForCurrentLibrarySegment];

        if (viewModeSegment == VLCLibraryListViewModeSegment) {
            [self presentAudioTableView];
        } else if (viewModeSegment == VLCLibraryGridViewModeSegment) {
            [self presentAudioGridModeView];
        } else {
            NSAssert(false, @"View mode must be grid or list mode");
        }

        [VLCMain.sharedInstance.libraryWindow updateGridVsListViewModeSegmentedControl];
    }
}

- (void)setCurrentSegmentType:(VLCLibrarySegmentType)currentSegmentType
{
    _currentSegmentType = currentSegmentType;
    [self updatePresentedView];
}

- (void)reloadData
{
    [_audioDataSource reloadData];
}

- (void)presentLibraryItemInTableView:(id<VLCMediaLibraryItemProtocol>)libraryItem
{
    if (libraryItem == nil) {
        return;
    }

    NSTableView *targetMainTableView;
    if ([libraryItem isKindOfClass:VLCMediaLibraryMediaItem.class]) {
        targetMainTableView = self.audioSongTableView;
    } else {
        targetMainTableView = self.audioCollectionSelectionTableView;
    }
    NSAssert(targetMainTableView != nil, @"Target tableview for presenting audio library view is nil");
    NSAssert(targetMainTableView.dataSource == self.audioDataSource, @"Target tableview data source is unexpected");

    const NSInteger rowForLibraryItem = [self.audioDataSource rowForLibraryItem:libraryItem];
    if (rowForLibraryItem != NSNotFound) {
        NSIndexSet * const indexSet = [NSIndexSet indexSetWithIndex:rowForLibraryItem];
        [targetMainTableView selectRowIndexes:indexSet byExtendingSelection:NO];
        [targetMainTableView scrollRowToVisible:rowForLibraryItem];
    }
}

- (void)presentLibraryItemInCollectionView:(id<VLCMediaLibraryItemProtocol>)libraryItem
{
    if (libraryItem == nil) {
        return;
    }

    // If we are handling a library item that is shown in one of the split view modes,
    // with a table view on the left and the collection view on the right, defer back
    // to presentLibraryItemInTabelView
    if ([libraryItem isKindOfClass:VLCMediaLibraryGenre.class] ||
        [libraryItem isKindOfClass:VLCMediaLibraryArtist.class]) {
        [self presentLibraryItemInTableView:libraryItem];
    }

    NSIndexPath * const indexPathForLibraryItem = [self.audioDataSource indexPathForLibraryItem:libraryItem];
    if (indexPathForLibraryItem) {
        NSSet<NSIndexPath *> * const indexPathSet = [NSSet setWithObject:indexPathForLibraryItem];
        NSCollectionView * const collectionView = self.audioLibraryCollectionView;
        VLCLibraryCollectionViewFlowLayout * const expandableFlowLayout = (VLCLibraryCollectionViewFlowLayout *)collectionView.collectionViewLayout;

        [collectionView selectItemsAtIndexPaths:indexPathSet
                                 scrollPosition:NSCollectionViewScrollPositionTop];
        [expandableFlowLayout expandDetailSectionAtIndex:indexPathForLibraryItem];
    }
}

- (void)presentLibraryItemWaitForDataSourceFinished:(NSNotification *)aNotification
{
    [NSNotificationCenter.defaultCenter removeObserver:self
                                                  name:VLCLibraryAudioDataSourceDisplayedCollectionChangedNotification
                                                object:self.audioDataSource];

    const VLCLibraryViewModeSegment viewModeSegment = [self viewModeSegmentForCurrentLibrarySegment];
    if (viewModeSegment == VLCLibraryListViewModeSegment) {
        [self presentLibraryItemInTableView:_awaitingPresentingLibraryItem];
    } else if (viewModeSegment == VLCLibraryGridViewModeSegment) {
        [self presentLibraryItemInCollectionView:_awaitingPresentingLibraryItem];
    }

    _awaitingPresentingLibraryItem = nil;
}

- (void)presentLibraryItem:(id<VLCMediaLibraryItemProtocol>)libraryItem
{
    if (libraryItem == nil) {
        return;
    }

    _awaitingPresentingLibraryItem = libraryItem;

    // If the library item is a media item, we need to select the corresponding segment
    // in the segmented control. We then need to update the presented view.
    if ([libraryItem isKindOfClass:VLCMediaLibraryAlbum.class]) {
        self.currentSegmentType = VLCLibraryAlbumsMusicSubSegment;
    } else if ([libraryItem isKindOfClass:VLCMediaLibraryArtist.class]) {
        self.currentSegmentType = VLCLibraryArtistsMusicSubSegment;
    } else if ([libraryItem isKindOfClass:VLCMediaLibraryGenre.class]) {
        self.currentSegmentType = VLCLibraryGenresMusicSubSegment;
    } else {
        self.currentSegmentType = VLCLibrarySongsMusicSubSegment;
    }

    [NSNotificationCenter.defaultCenter addObserver:self
                                           selector:@selector(presentLibraryItemWaitForDataSourceFinished:)
                                               name:VLCLibraryAudioDataSourceDisplayedCollectionChangedNotification
                                             object:self.audioDataSource];

    [self updatePresentedView];
    if (!self.audioDataSource.displayedCollectionUpdating) {
        [self presentLibraryItemWaitForDataSourceFinished:nil];
    }
}

- (void)libraryModelUpdated:(NSNotification *)aNotification
{
    NSParameterAssert(aNotification);
    VLCLibraryModel *model = (VLCLibraryModel *)aNotification.object;
    NSAssert(model, @"Notification object should be a VLCLibraryModel");
    const NSUInteger audioCount = model.numberOfAudioMedia;

    if ((self.libraryWindow.librarySegmentType == VLCLibraryMusicSegment ||
         self.libraryWindow.librarySegmentType == VLCLibrarySongsMusicSubSegment ||
         self.libraryWindow.librarySegmentType == VLCLibraryArtistsMusicSubSegment ||
         self.libraryWindow.librarySegmentType == VLCLibraryAlbumsMusicSubSegment ||
         self.libraryWindow.librarySegmentType == VLCLibraryGenresMusicSubSegment) &&
        ((audioCount == 0 && ![_libraryTargetView.subviews containsObject:_emptyLibraryView]) ||
         (audioCount > 0 && ![_libraryTargetView.subviews containsObject:_audioLibraryView])) &&
        _libraryWindow.videoViewController.view.hidden) {

        [self updatePresentedView];
    }
}

@end
