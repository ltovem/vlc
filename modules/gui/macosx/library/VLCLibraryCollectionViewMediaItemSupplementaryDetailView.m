// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLibraryCollectionViewMediaItemSupplementaryDetailView.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Claudio Cambra <claudio.cambra@gmail.com>
 *****************************************************************************/

#import "VLCLibraryCollectionViewMediaItemSupplementaryDetailView.h"

#import "main/VLCMain.h"
#import "library/VLCInputItem.h"
#import "library/VLCLibraryController.h"
#import "library/VLCLibraryDataTypes.h"
#import "library/VLCLibraryModel.h"
#import "library/VLCLibraryMenuController.h"
#import "views/VLCImageView.h"
#import "extensions/NSString+Helpers.h"
#import "extensions/NSFont+VLCAdditions.h"
#import "extensions/NSColor+VLCAdditions.h"
#import "extensions/NSView+VLCAdditions.h"

NSString *const VLCLibraryCollectionViewMediaItemSupplementaryDetailViewIdentifier = @"VLCLibraryCollectionViewMediaItemSupplementaryDetailViewIdentifier";
NSCollectionViewSupplementaryElementKind const VLCLibraryCollectionViewMediaItemSupplementaryDetailViewKind = @"VLCLibraryCollectionViewMediaItemSupplementaryDetailViewIdentifier";

@interface VLCLibraryCollectionViewMediaItemSupplementaryDetailView ()
{
    VLCLibraryController *_libraryController;
}

@end

@implementation VLCLibraryCollectionViewMediaItemSupplementaryDetailView

- (void)awakeFromNib
{
    _mediaItemTitleTextField.font = [NSFont VLCLibrarySupplementaryDetailViewTitleFont];
    _mediaItemDetailTextField.font = [NSFont VLCLibrarySupplementaryDetailViewSubtitleFont];
    _mediaItemDetailTextField.textColor = [NSColor VLCAccentColor];

    if(@available(macOS 10.12.2, *)) {
        _playMediaItemButton.bezelColor = [NSColor VLCAccentColor];
    }
}

- (void)setRepresentedMediaItem:(VLCMediaLibraryMediaItem *)representedMediaItem
{
    _representedMediaItem = representedMediaItem;
    [self updateRepresentation];
}

- (NSString*)formattedYearAndDurationString
{
    if (_representedMediaItem.year > 0) {
        return [NSString stringWithFormat:@"%u · %@", _representedMediaItem.year, _representedMediaItem.durationString];
    } else if (_representedMediaItem.files.count > 0) {
        VLCMediaLibraryFile *firstFile = _representedMediaItem.files.firstObject;
        time_t fileLastModTime = firstFile.lastModificationDate;

        if (fileLastModTime > 0) {
            NSDate *lastModDate = [NSDate dateWithTimeIntervalSince1970:fileLastModTime];
            NSDateComponents *components = [[NSCalendar currentCalendar] components:NSCalendarUnitYear fromDate:lastModDate];
            return [NSString stringWithFormat:@"%ld · %@", components.year, _representedMediaItem.durationString];
        }
    }

    return _representedMediaItem.durationString;
}

- (void)updateRepresentation
{
    if (_representedMediaItem == nil) {
        NSAssert(1, @"no media item assigned for collection view item", nil);
        return;
    }

    _mediaItemTitleTextField.stringValue = _representedMediaItem.displayString;
    _mediaItemDetailTextField.stringValue = _representedMediaItem.detailString;
    _mediaItemYearAndDurationTextField.stringValue = [self formattedYearAndDurationString];
    _mediaItemFileNameTextField.stringValue = _representedMediaItem.inputItem.name;
    _mediaItemPathTextField.stringValue = _representedMediaItem.inputItem.decodedMRL;
    _mediaItemArtworkImageView.image = _representedMediaItem.smallArtworkImage;
}

- (IBAction)playAction:(id)sender
{
    if (!_libraryController) {
        _libraryController = [[VLCMain sharedInstance] libraryController];
    }

    [_libraryController appendItemToPlaylist:_representedMediaItem playImmediately:YES];
}

- (IBAction)enqueueAction:(id)sender
{
    if (!_libraryController) {
        _libraryController = [[VLCMain sharedInstance] libraryController];
    }

    [_libraryController appendItemToPlaylist:_representedMediaItem playImmediately:NO];
}

@end
