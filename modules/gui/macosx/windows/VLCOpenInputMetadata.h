// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCOpenInputMetadata.h: macOS interface
 *****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne # videolan dot org>
 *****************************************************************************/

#import <Foundation/Foundation.h>

@interface VLCOpenInputMetadata : NSObject

/**
 * Create a new VLCOpenInputMetadata with the given file path
 *
 * See \c -initWithPath: for details.
 */
+ (instancetype)inputMetaWithPath:(NSString *)path;

/**
 * Initialize the VLCOpenInputMetadata  with the given file path
 *
 * Initializes the new VLCOpenInputMetadata with the MRLString
 * referring to the file given by path. Note that it is not
 * verified that the file actually exists, so it will succeed
 * regardless of the presence of the file.
 */
- (instancetype)initWithPath:(NSString *)path;

/**
 * this is the MRL of the future input item and defines where to search for media
 * it is the only required property, because if unset we don't know what to play
 */
@property (readwrite, copy) NSString *MRLString;

/**
 * this is an optional property to define the item name
 * if not set, the MRL or (if suitable) a file name will be displayed to the user
 */
@property (readwrite, copy) NSString *itemName;

/**
 * this is an optional property to define custom playback options
 * this typically relies on VLC's private API and shall be considered potentially unstable
 */
@property (readwrite, copy) NSArray *playbackOptions;

@end
