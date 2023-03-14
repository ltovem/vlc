/*****************************************************************************
 * VLCLibraryTableCellView.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCImageView;
@class VLCTrackingView;
@protocol VLCMediaLibraryItemProtocol;
@class VLCInputItem;

@interface VLCLibraryTableCellView : NSTableCellView

+ (instancetype)fromNibWithOwner:(id)owner;

@property (readwrite, assign) IBOutlet VLCTrackingView *trackingView;
@property (readwrite, assign) IBOutlet NSTextField *singlePrimaryTitleTextField;
@property (readwrite, assign) IBOutlet NSTextField *secondaryTitleTextField;
@property (readwrite, assign) IBOutlet NSTextField *primaryTitleTextField;
@property (readwrite, assign) IBOutlet VLCImageView *representedImageView;
@property (readwrite, assign) IBOutlet NSButton *playInstantlyButton;

@property (readwrite, strong, nonatomic) id<VLCMediaLibraryItemProtocol> representedItem;
@property (readwrite, strong, nonatomic) VLCInputItem *representedInputItem;
@property (readwrite, nonatomic) NSUInteger representedVideoLibrarySection;

@end

NS_ASSUME_NONNULL_END
