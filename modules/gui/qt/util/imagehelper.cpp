// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * input_slider.cpp : VolumeSlider and SeekSlider
 *****************************************************************************
 * Copyright (C) 2006-2017 the VideoLAN team
 *
 * Authors: Pierre Lamot <pierre@videolabs.io>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt.hpp"
#include <QApplication>
#include <QPainter>
#include <QScreen>
#include <QSvgRenderer>
#include "imagehelper.hpp"


QPixmap ImageHelper::loadSvgToPixmap( const QString &path, qint32 i_width, qint32 i_height )
{
    qreal ratio = QApplication::primaryScreen()->devicePixelRatio();
    QPixmap pixmap( QSize( i_width, i_height ) * ratio );


    pixmap.fill( Qt::transparent );

    QSvgRenderer renderer( path );
    QPainter painter;

    painter.begin( &pixmap );
    renderer.render( &painter );
    painter.end();

    pixmap.setDevicePixelRatio( ratio );
    return pixmap;
}
