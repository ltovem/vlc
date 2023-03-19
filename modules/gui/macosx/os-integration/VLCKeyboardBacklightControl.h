// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCKeyboardBlacklightControl.h: MacBook keyboard backlight control for VLC
 *****************************************************************************
 * Copyright (C) 2015 VLC authors and VideoLAN
 *
 *
 * Authors: Maxime Mouchet <max@maxmouchet.com>
 *****************************************************************************/

#import <Foundation/Foundation.h>

@interface VLCKeyboardBacklightControl : NSObject

/*!
 *  Smoothly turn on backlight on MacBook keyboard.
 *
 *  Try to restore the previous brightness level. If the user has put
 *  the backlight on manually (using hardware keys), nothing will be done.
 */
- (void)lightsUp;

/*!
 *  Smoothly turn off backlight on MacBook keyboard.
 */
- (void)lightsDown;

/*!
 *  Smoothly switch on or off backlight on MacBook keyboard.
 *  This will return immediately.
 */
- (void)switchLightsAsync:(BOOL)on;

/*!
 *  Instantly switch on or off backlight on MacBook keyboard.
 */
- (void)switchLightsInstantly:(BOOL)on;

@end
