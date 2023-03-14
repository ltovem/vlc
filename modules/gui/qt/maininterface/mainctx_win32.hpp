/*****************************************************************************
 * mainctx_win32.cpp : Main interface
 ****************************************************************************
 * Copyright (C) 2006-2010 VideoLAN and AUTHORS
 *
 * Authors: Jean-Baptiste Kempf <jb@videolan.org>
 *          Hugo Beauzée-Luyssen <hugo@beauzee.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MAIN_INTERFACE_WIN32_HPP
#define MAIN_INTERFACE_WIN32_HPP

#include "maininterface/mainctx.hpp"
#include "interface_window_handler.hpp"
#include <QAbstractNativeEventFilter>

class WinTaskbarWidget : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    WinTaskbarWidget( qt_intf_t *p_intf, QWindow* windowHandle, QObject* parent = nullptr);
    virtual ~WinTaskbarWidget();

private:
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;
    void createTaskBarButtons();

private slots:
    void changeThumbbarButtons(PlayerController::PlayingState );
    void playlistItemCountChanged( size_t itemId );
    virtual void onVideoFullscreenChanged( bool fs );

private:
    qt_intf_t* p_intf = nullptr;
    HIMAGELIST himl = nullptr;
    ITaskbarList3 *p_taskbl = nullptr;
    UINT taskbar_wmsg = 0;
    QWindow* m_window = nullptr;

};



class MainCtxWin32 : public MainCtx
{
    Q_OBJECT
public:
    explicit MainCtxWin32(qt_intf_t *);
    virtual ~MainCtxWin32() = default;

public slots:
    virtual void reloadPrefs() override;
};

class InterfaceWindowHandlerWin32 : public InterfaceWindowHandler, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit InterfaceWindowHandlerWin32(qt_intf_t *_p_intf, MainCtx* mainCtx, QWindow* window, QWidget* widget, QObject *parent = nullptr);
    virtual ~InterfaceWindowHandlerWin32();
    virtual void toggleWindowVisibility() override;

    virtual bool eventFilter(QObject*, QEvent* event) override;

protected:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) override;

private:
#if QT_CLIENT_SIDE_DECORATION_AVAILABLE
    void updateCSDWindowSettings() override;
    QObject *m_CSDWindowEventHandler {};
#endif
};

#endif // MAIN_INTERFACE_WIN32_HPP
