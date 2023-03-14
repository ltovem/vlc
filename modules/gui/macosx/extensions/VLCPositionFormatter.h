/*****************************************************************************
 * VLCPositionFormatter.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2013, 2019 VLC authors and VideoLAN
 *
 * Author: David Fuhrmann <dfuhrmann # videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Foundation/Foundation.h>

@interface VLCPositionFormatter : NSFormatter

- (NSString*)stringForObjectValue:(id)obj;

- (BOOL)getObjectValue:(id*)obj
             forString:(NSString*)string
      errorDescription:(NSString**)error;

- (BOOL)isPartialStringValid:(NSString*)partialString
            newEditingString:(NSString**)newString
            errorDescription:(NSString**)error;

@end
