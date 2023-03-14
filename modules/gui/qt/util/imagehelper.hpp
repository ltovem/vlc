/*****************************************************************************
 * input_slider.cpp : VolumeSlider and SeekSlider
 ****************************************************************************
 * Copyright (C) 2006-2017 the VideoLAN team
 *
 * Authors: Pierre Lamot <pierre@videolabs.io>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef IMAGEHELPER_HPP
#define IMAGEHELPER_HPP

#include <QString>
#include <QPixmap>

class ImageHelper
{
public:
    /* render a Svg to a pixmap with current device pixel ratio */
    static QPixmap loadSvgToPixmap(const QString& path, qint32 i_width, qint32 i_height);
};

#endif // IMAGEHELPER_HPP
