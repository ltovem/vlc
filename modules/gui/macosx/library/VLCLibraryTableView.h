// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryTableView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@protocol VLCMediaLibraryItemProtocol;

@protocol VLCLibraryTableViewDataSource <NSTableViewDataSource>

- (id<VLCMediaLibraryItemProtocol>)libraryItemAtRow:(NSInteger)row
                                       forTableView:(NSTableView *)tableView;

@end

@interface VLCLibraryTableView : NSTableView<NSMenuDelegate>

@end

NS_ASSUME_NONNULL_END
