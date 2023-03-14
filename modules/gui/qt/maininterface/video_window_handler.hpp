/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef VIDEOWINDOWHANDLER_HPP
#define VIDEOWINDOWHANDLER_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>

#include <QWindow>
#include <QObject>

#include "qt.hpp"

class MainCtx;

class VideoWindowHandler : public QObject
{
    Q_OBJECT
public:
    explicit VideoWindowHandler(qt_intf_t *intf, QObject *parent = nullptr);

public:
    void setWindow(QWindow* window);

    void disable();
    void requestResizeVideo( unsigned, unsigned );
    void requestVideoState( unsigned );
    void requestVideoWindowed( );
    void requestVideoFullScreen( const char * );

signals:
    void askVideoToResize( unsigned int, unsigned int );
    void askVideoSetFullScreen( bool );
    void askVideoOnTop( bool );

protected slots:
    /* Manage the Video Functions from the vout threads */
    void setVideoSize(unsigned int w, unsigned int h);
    virtual void setVideoFullScreen( bool );
    void setVideoOnTop( bool );

private:
    qt_intf_t *m_intf = nullptr;
    QWindow* m_window = nullptr;

    bool m_videoFullScreen = false;
    bool m_autoresize = false;

    QRect   m_lastWinGeometry;
    QScreen* m_lastWinScreen = nullptr;

#ifdef QT5_HAS_WAYLAND
    bool m_hasWayland = false;
#endif
};

#endif // VIDEOWINDOWHANDLER_HPP
