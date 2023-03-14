/*****************************************************************************
 * EPGRuler.hpp: EPGRuler
 ****************************************************************************
 * Copyright © 2009-2010 VideoLAN
 *
 * Authors: Ludovic Fauvet <etix@l0cal.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EPGRULER_H
#define EPGRULER_H

#include "qt.hpp"

#include <QWidget>
#include <QDateTime>

class EPGRuler : public QWidget
{
    Q_OBJECT

public:
    EPGRuler( QWidget* parent = 0 );
    virtual ~EPGRuler() { }

public slots:
    void setScale( double scale );
    void setRange( const QDateTime&, const QDateTime& );
    void setOffset( int offset );

protected:
    void paintEvent( QPaintEvent *event ) override;

private:
    qreal m_scale;
    int m_duration;
    int m_offset;
    QDateTime m_startTime;
};

#endif // EPGRULER_H
