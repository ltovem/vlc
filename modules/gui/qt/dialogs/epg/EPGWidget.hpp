// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * EPGWidget.hpp : EPGWidget
 *****************************************************************************
 * Copyright © 2009-2010 VideoLAN
 *
 * Authors: Ludovic Fauvet <etix@l0cal.com>
 *****************************************************************************/

#ifndef EPGWIDGET_H
#define EPGWIDGET_H

#include "qt.hpp"

#include <vlc_epg.h>
#include <vlc_input_item.h>

#include <QWidget>

class QStackedWidget;

class EPGView;
class EPGItem;
class EPGRuler;
class EPGChannels;

class EPGWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EPGWidget( QWidget* parent = 0 );
    void reset();
    enum
    {
        EPGVIEW_WIDGET = 0,
        NOEPG_WIDGET = 1
    };

public slots:
    void setZoom( int level );
    void updateEPG( input_item_t * );
    void activateProgram( int );

private:
    EPGRuler* m_rulerWidget;
    EPGView* m_epgView;
    EPGChannels *m_channelsWidget;
    QStackedWidget *rootWidget;

    enum input_item_type_e i_event_source_type;
    bool b_input_type_known;

signals:
    void itemSelectionChanged( EPGItem * );
    void programActivated( int );
};

#endif // EPGWIDGET_H
