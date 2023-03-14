/*****************************************************************************
 * CompatibilityFixes.h: MacOS X interface module
 *****************************************************************************
 * Copyright (C) 2011-2017 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne -at- videolan -dot- org>
 *          Marvin Scholz <epirat07 -at- gmail -dot- com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "CompatibilityFixes.h"
#import <objc/runtime.h>

/**
 Swaps out the implementation of the method at @c selector in Class @c cls
 with the implementation of that method from the superclass.

 @param cls         The class which this selector belongs to
 @param selector    The selector of whom to swap the implementation

 @note  The @c cls must be a subclass of another class and both
        must implement the @c selector for this function to work as expected!
 */
void swapoutOverride(Class cls, SEL selector)
{
    Method subclassMeth = class_getInstanceMethod(cls, selector);
    IMP baseImp = class_getMethodImplementation([cls superclass], selector);

    if (subclassMeth && baseImp)
        method_setImplementation(subclassMeth, baseImp);
}

#ifndef MAC_OS_X_VERSION_10_14

NSString *const NSAppearanceNameDarkAqua = @"NSAppearanceNameDarkAqua";

#endif

#ifndef MAC_OS_X_VERSION_10_13

NSString *const NSCollectionViewSupplementaryElementKind = @"NSCollectionViewSupplementaryElementKind";

#endif
