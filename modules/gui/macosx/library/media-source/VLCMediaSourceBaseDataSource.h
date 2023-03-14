/*****************************************************************************
 * VLCMediaSourceBaseDataSource.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

typedef NS_ENUM(NSInteger, VLCMediaSourceMode) {
    VLCMediaSourceModeLAN,
    VLCMediaSourceModeInternet,
};

NS_ASSUME_NONNULL_BEGIN

@class VLCInputNodePathControl;
@class VLCMediaSourceDataSource;

@interface VLCMediaSourceBaseDataSource : NSObject <NSCollectionViewDataSource,
                                                    NSCollectionViewDelegate,
                                                    NSCollectionViewDelegateFlowLayout,
                                                    NSTableViewDelegate,
                                                    NSTableViewDataSource>

@property (readwrite) NSCollectionView *collectionView;
@property (readwrite) NSScrollView *collectionViewScrollView;
@property (readwrite) NSTableView *tableView;
@property (readwrite) NSSegmentedControl *gridVsListSegmentedControl;
@property (readwrite) NSButton *homeButton;
@property (readwrite) VLCInputNodePathControl *pathControl;
@property (readwrite, nonatomic) VLCMediaSourceMode mediaSourceMode;
@property (readwrite, nonatomic) VLCMediaSourceDataSource *childDataSource;

- (void)setupViews;
- (void)reloadViews;
- (void)homeButtonAction:(id)sender;
- (void)pathControlAction:(id)sender;
- (void)setGridOrListMode:(id)sender;

@end

NS_ASSUME_NONNULL_END
