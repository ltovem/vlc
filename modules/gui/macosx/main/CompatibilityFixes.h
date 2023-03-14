/*****************************************************************************
 * CompatibilityFixes.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2011-2018 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *          Marvin Scholz <epirat07 -at- gmail -dot- com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

#pragma mark -
void swapoutOverride(Class _Nonnull cls, SEL _Nonnull selector);

#ifndef MAC_OS_X_VERSION_10_14

extern NSString *const NSAppearanceNameDarkAqua;

#endif

#ifndef MAC_OS_X_VERSION_10_13

extern NSString *const NSCollectionViewSupplementaryElementKind;

#endif

NS_ASSUME_NONNULL_END

#define OSX_BIGSUR_AND_HIGHER (NSAppKitVersionNumber >= 2022.00)
