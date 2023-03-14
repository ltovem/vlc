/*****************************************************************************
 * VLCDefaultValueSliderCell.h: SliderCell subclass for VLCDefaultValueSlider
 *****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 *
 * Authors: Marvin Scholz <epirat07 -at- gmail -dot- com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import <Cocoa/Cocoa.h>

/**
 \c VLCDefaultValueSliderCell is the cell use by the
 \c VLCDefaultValueSlider class.
 */
@interface VLCDefaultValueSliderCell : NSSliderCell

/**
 Indicates if a tickmark should be drawn for the \c defaultValue
 */
@property (readwrite) BOOL drawTickMarkForDefault;

/**
 Indicates if the slider knob should snap to the \c defaultValue
 */
@property (readwrite) BOOL snapsToDefault;

/**
 The default value of the slider

 \note It may not be equal to \c DBL_MAX, as this is the value
       that it should be set to, if no defaultValue is desired.
 */
@property (getter=defaultValue, setter=setDefaultValue:) double defaultValue;

/**
 Color of the default tick mark
 */
@property (getter=defaultTickMarkColor, setter=setDefaultTickMarkColor:) NSColor *defaultTickMarkColor;

/**
 Draws the tick mark for the \c defaultValue in the
 given rect.

 \note Override this in a subclass if you need to customize the
 tickmark that is drawn for the \c defaultValue

 \param rect The rect in which the tickMark should be drawn
 */
- (void)drawDefaultTickMarkWithFrame:(NSRect)rect;

@end
