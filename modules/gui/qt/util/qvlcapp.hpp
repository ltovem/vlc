/*****************************************************************************
 * qvlcapp.hpp : A few helpers
 *****************************************************************************
 * Copyright (C) 2008 the VideoLAN team
 *
 * Authors: Jean-Baptiste Kempf <jb@videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/


#ifndef VLC_QT_QVLCAPP_HPP_
#define VLC_QT_QVLCAPP_HPP_

#include <QApplication>
#include <QStyle>

#if defined(Q_OS_WIN)
#   include "qt.hpp"
#   include <windows.h>
#   include "player/player_controller.hpp"
#endif

class QVLCApp : public QApplication
{
    Q_OBJECT

private slots:
    void doQuit()
    {
        closeAllWindows();
        quit();
    }

public:
    QVLCApp( int & argc, char ** argv ) : QApplication( argc, argv, true ), m_defaultStyle( style()->objectName() )
    {
        connect( this, &QVLCApp::quitSignal, this, &QVLCApp::doQuit );
    }

    static void triggerQuit()
    {
        QVLCApp *app = qobject_cast<QVLCApp*>( instance() );
        if( app )
            emit app->quitSignal();
    }

    QString defaultStyle() const
    {
        return m_defaultStyle;
    }

signals:
    void quitSignal();

private:
    const QString m_defaultStyle;
};

#endif
