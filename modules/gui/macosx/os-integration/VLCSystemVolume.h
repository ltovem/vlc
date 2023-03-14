/* Copyright (C) 2003-2020 VLC authors and VideoLAN
 *
 * Authors: Jon Lech Johansen <jon-vl@nanocrew.net>
 *          Felix Paul Kühne <fkuehne at videolan dot org>
 *          Marvin Scholz <epirat07 at gmail dot com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 Constant to use with \c changeSystemVolume to increase the volume by the default amount
 */
static const Float32 kVLCSystemVolumeIncreaseDefault = +0.0625; // 1/16 to match the OS

/**
 Constant to use with \c changeSystemVolume to decrease the volume by the default amount
 */
static const Float32 kVLCSystemVolumeDecreaseDefault = -0.0625; // 1/16 to match the OS

/**
 Class which provides helpers to manage the system default output device volume

 This class can not be instantiated, it only provides class methods that help to manage the volume of the
 systems default audio output device.
*/
@interface VLCSystemVolume : NSObject

+ (instancetype)new NS_UNAVAILABLE;
- (instancetype)init NS_UNAVAILABLE;

/**
 Change the system volume by the given amount

 Changes the system volume, which is the volume of the current default audio
 output device by the given amount.
 \note Failures in this method are logged, in addition to the error return.

 \param amount  Amount by which to increase (positive value) or decrease (negative value)
                the current volume. (Use \c kVLCSystemVolumeIncreaseDefault or
                \c kVLCSystemVolumeDecreaseDefault for the default amounts)

 \return \c YES on success or \c NO on failure.
 */
+ (BOOL)changeSystemVolume:(Float32)amount;

@end

NS_ASSUME_NONNULL_END
