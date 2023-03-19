// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * VLCWrappableTextField.h
 *****************************************************************************
 * Copyright (C) 2017 VideoLAN and authors
 * Author:       David Fuhrmann <dfuhrmann at videolan dot org>
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

/**
 * Helper class for wrappable text multi line text fields on 10.7.
 *
 * Makes sure to try to wrap the text while calculating an intrinsic size for
 * the field.
 *
 * For this to work, make sure that:
 * - Field has a minimum width (best is to use >= constraint)
 * - Field has layout set to wrap
 * - Fields preferred with setting is explicit with constant 0 (auto or runtime
 *   width are not compatible with 10.7)
 * - If text can change, make sure to have vertical hugging priorities > 500 so
 *   that window height can shrink again if text gets smaller.
 *
 * TODO: Revisit that code one 10.7 is dropped.
 */
@interface VLCWrappableTextField : NSTextField

@end