// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewAudioGroupSupplementaryDetailView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

#import "library/VLCLibraryCollectionViewSupplementaryDetailView.h"

NS_ASSUME_NONNULL_BEGIN

@protocol VLCMediaLibraryAudioGroupProtocol;
@class VLCImageView;

extern NSString *const VLCLibraryCollectionViewAudioGroupSupplementaryDetailViewIdentifier;
extern NSCollectionViewSupplementaryElementKind const VLCLibraryCollectionViewAudioGroupSupplementaryDetailViewKind;

@interface VLCLibraryCollectionViewAudioGroupSupplementaryDetailView : VLCLibraryCollectionViewSupplementaryDetailView

@property (readwrite, retain, nonatomic) id<VLCMediaLibraryAudioGroupProtocol> representedAudioGroup;
@property (readwrite, weak) IBOutlet NSTextField *audioGroupNameTextField;
@property (readwrite, weak) IBOutlet NSTableView *audioGroupAlbumsTableView;
@property (readwrite, weak) IBOutlet NSClipView *tableClipView;
@property (readwrite, weak) IBOutlet NSScrollView *tableScrollView;

@end

NS_ASSUME_NONNULL_END
