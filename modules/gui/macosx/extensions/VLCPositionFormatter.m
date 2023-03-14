/*****************************************************************************
 * VLCPositionFormatter.m: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2013, 2019 VLC authors and VideoLAN
 *
 * Author: David Fuhrmann <dfuhrmann # videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "VLCPositionFormatter.h"

@interface VLCPositionFormatter()
{
    NSCharacterSet *o_forbidden_characters;
}
@end

@implementation VLCPositionFormatter

- (id)init
{
    self = [super init];
    if (self) {
        NSMutableCharacterSet *nonNumbers = [[[NSCharacterSet decimalDigitCharacterSet] invertedSet] mutableCopy];
        [nonNumbers removeCharactersInString:@"-:"];
        o_forbidden_characters = [nonNumbers copy];
    }
    return self;
}

- (NSString*)stringForObjectValue:(id)obj
{
    if([obj isKindOfClass:[NSString class]])
        return obj;
    if([obj isKindOfClass:[NSNumber class]])
        return [obj stringValue];

    return nil;
}

- (BOOL)getObjectValue:(id*)obj forString:(NSString*)string errorDescription:(NSString**)error
{
    *obj = [string copy];
    return YES;
}

- (BOOL)isPartialStringValid:(NSString*)partialString newEditingString:(NSString**)newString errorDescription:(NSString**)error
{
    if ([partialString rangeOfCharacterFromSet:o_forbidden_characters options:NSLiteralSearch].location != NSNotFound) {
        return NO;
    } else {
        return YES;
    }
}

@end
