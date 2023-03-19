// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewItem.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan -dot- org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@class VLCImageView;
@class VLCLinearProgressIndicator;
@protocol VLCMediaLibraryItemProtocol;

extern NSString *VLCLibraryCellIdentifier;

@interface VLCLibraryCollectionViewItem : NSCollectionViewItem

// NOTE: These will need to be changed after changes to XIB
+ (const NSSize)defaultSize;
+ (const NSSize)defaultVideoItemSize;
+ (const CGFloat)defaultWidth;
+ (const CGFloat)bottomTextViewsHeight;
+ (const CGFloat)videoHeightAspectRatioMultiplier;

@property (readwrite, assign) IBOutlet NSTextField *mediaTitleTextField;
@property (readwrite, assign) IBOutlet NSTextField *annotationTextField;
@property (readwrite, assign) IBOutlet NSTextField *unplayedIndicatorTextField;
@property (readwrite, assign) IBOutlet NSTextField *secondaryInfoTextField;
@property (readwrite, assign) IBOutlet VLCImageView *mediaImageView;
@property (readwrite, assign) IBOutlet NSButton *playInstantlyButton;
@property (readwrite, assign) IBOutlet NSButton *addToPlaylistButton;
@property (readwrite, assign) IBOutlet VLCLinearProgressIndicator *progressIndicator;
@property (readwrite, assign) IBOutlet NSBox *highlightBox;
@property (readwrite, assign) IBOutlet NSLayoutConstraint *imageViewAspectRatioConstraint;

@property (readwrite, retain, nonatomic) id<VLCMediaLibraryItemProtocol> representedItem;

@end

NS_ASSUME_NONNULL_END
