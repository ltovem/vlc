/*****************************************************************************
 * VLCLibraryTableView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCLibraryTableView.h"
#import "library/VLCLibraryDataTypes.h"
#import "library/VLCLibraryMenuController.h"
#import "media-source/VLCMediaSourceDataSource.h"

@interface VLCLibraryTableView ()
{
    VLCLibraryMenuController *_menuController;
    BOOL _vlcDataSourceConforming;
}
@end

@implementation VLCLibraryTableView

- (void)setupMenu
{
    if(_menuController == nil) {
        _menuController = [[VLCLibraryMenuController alloc] init];
        _menuController.libraryMenu.delegate = self;
    }

    self.menu = _menuController.libraryMenu;
}

- (void)setDataSource:(id<NSTableViewDataSource>)dataSource
{
    [super setDataSource:dataSource];

    if([self.dataSource conformsToProtocol:@protocol(VLCLibraryTableViewDataSource)] ||
       self.dataSource.class == VLCMediaSourceDataSource.class) {
        _vlcDataSourceConforming = YES;
        [self setupMenu];
    } else {
        _vlcDataSourceConforming = NO;
        self.menu = nil;
        _menuController = nil;
    }
}

#pragma mark tableview menu delegates

- (void)menuNeedsUpdate:(NSMenu *)menu
{
    if(self.clickedRow < 0 || self.dataSource == nil || !_vlcDataSourceConforming) {
        return;
    }

    if([self.dataSource conformsToProtocol:@protocol(VLCLibraryTableViewDataSource)]) {
        id<VLCLibraryTableViewDataSource> vlcLibraryDataSource = (id<VLCLibraryTableViewDataSource>)self.dataSource;
        id<VLCMediaLibraryItemProtocol> mediaLibraryItem = [vlcLibraryDataSource libraryItemAtRow:self.clickedRow
                                                                                     forTableView:self];
        [_menuController setRepresentedItem:mediaLibraryItem];
    } else if (self.dataSource.class == VLCMediaSourceDataSource.class) {
        VLCMediaSourceDataSource *mediaSourceDataSource = (VLCMediaSourceDataSource*)self.dataSource;
        NSAssert(mediaSourceDataSource != nil, @"This should be a valid pointer");
        VLCInputItem *mediaSourceInputItem = [mediaSourceDataSource mediaSourceInputItemAtRow:self.clickedRow];
        [_menuController setRepresentedInputItem:mediaSourceInputItem];
    }
}

@end
