// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCLogMessage.m: Log message class
 *****************************************************************************
 * Copyright (C) 2017 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 at gmail dot com>
 *****************************************************************************/

#import "VLCLogMessage.h"

@implementation VLCLogMessage

+ (instancetype)logMessage:(char *)msg type:(int)type info:(const vlc_log_t *)info
{
    return [[VLCLogMessage alloc] initWithMessage:msg type:type info:info];
}

- (instancetype)initWithMessage:(char *)message type:(int)type info:(const vlc_log_t *)info
{
    self = [super init];
    if (self) {
        if (!message || !info->psz_module)
            return nil;

        _type = type;
        _message = [NSString stringWithUTF8String:message];
        _location = [NSString stringWithFormat:@"%s:%i", info->file, info->line];
        _component = [NSString stringWithUTF8String:info->psz_module];
        _function = [NSString stringWithUTF8String:info->func];
    }
    return self;
}

- (NSString *)fullMessage
{
    return [NSString stringWithFormat:@"%@ %@: %@", _component, self.typeName, _message];
}

- (NSString *)typeName
{
    switch (_type) {
        case VLC_MSG_INFO:
            return @"info";
        case VLC_MSG_ERR:
            return @"error";
        case VLC_MSG_WARN:
            return @"warning";
        case VLC_MSG_DBG:
            return @"debug";
        default:
            return @"unknown";
    }
}

@end
