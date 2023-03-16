// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCMediaSourceDataSource.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCInputItem;
@class VLCInputNode;
@class VLCInputNodePathControl;
@class VLCMediaSource;

@interface VLCMediaSourceDataSource : NSObject <NSCollectionViewDataSource,
                                                NSCollectionViewDelegate,
                                                NSCollectionViewDelegateFlowLayout,
                                                NSTableViewDelegate,
                                                NSTableViewDataSource>

@property (readwrite, retain) VLCMediaSource *displayedMediaSource;
@property (readwrite, retain, nonatomic) VLCInputNode *nodeToDisplay;
@property (readwrite, assign) NSCollectionView *collectionView;
@property (readwrite, assign) NSTableView *tableView;
@property (readwrite) VLCInputNodePathControl *pathControl;
@property (readwrite) BOOL gridViewMode;

- (void)setupViews;
- (VLCInputItem*)mediaSourceInputItemAtRow:(NSInteger)tableViewRow;

@end

NS_ASSUME_NONNULL_END
