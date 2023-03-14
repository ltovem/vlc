/*****************************************************************************
 * info_widgets.hpp : Widgets for info panels
 ****************************************************************************
 * Copyright (C) 2013 the VideoLAN team
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef INFO_WIDGETS_HPP
#define INFO_WIDGETS_HPP

#include <QGraphicsView>

class QGraphicsScene;
class QGraphicsPolygonItem;
class QGraphicsLineItem;

class VLCStatsView: public QGraphicsView
{
    Q_OBJECT

public:
    VLCStatsView( QWidget * );
    void addValue( float );
    void reset();

private:
    void addHistoryValue( float );
    void drawRulers( const QRectF & );
    QGraphicsScene *viewScene;
    QGraphicsPolygonItem *totalbitrateShape;
    QGraphicsPolygonItem *historyShape;
    QGraphicsLineItem *rulers[3];
    unsigned int historymergepointer;
    unsigned int blocksize;
    float valuesaccumulator;
    unsigned int valuesaccumulatorcount;
};

#endif // INFO_WIDGETS_HPP
