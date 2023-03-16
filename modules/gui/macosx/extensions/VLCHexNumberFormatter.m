// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCHexNumberFormatter.m
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import "VLCHexNumberFormatter.h"

@implementation VLCHexNumberFormatter

- (NSString *)stringForObjectValue:(id)obj
{
    if (![obj isKindOfClass:[NSNumber class]]) {
        return nil;
    }

    NSString *string = [NSString stringWithFormat:@"%06" PRIX32, [obj intValue]];
    return string;
}

- (BOOL)getObjectValue:(out id  _Nullable __autoreleasing *)obj
             forString:(NSString *)string
      errorDescription:(out NSString *__autoreleasing  _Nullable *)error
{
    unsigned result = 0;
    NSScanner *scanner = [NSScanner scannerWithString:string];
    BOOL success = [scanner scanHexInt:&result];

    *obj = (success) ? [NSNumber numberWithInt:result] : nil;
    return success;
}

@end
