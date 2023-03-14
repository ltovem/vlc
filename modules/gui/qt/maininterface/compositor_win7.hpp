/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef COMPOSITORDUMMYWIN32_H
#define COMPOSITORDUMMYWIN32_H

#include "compositor_dummy.hpp"
#include "videosurface.hpp"
#include "video_window_handler.hpp"
#include <QAbstractNativeEventFilter>
#include <memory>

class WinTaskbarWidget;
class InterfaceWindowHandlerWin32;

namespace vlc {

class Win7NativeEventFilter : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT
public:
    Win7NativeEventFilter( QObject* parent = nullptr );

    bool nativeEventFilter(const QByteArray &, void *message, long* /* result */);
signals:
    void windowStyleChanged();
};

class CompositorWin7 : public CompositorVideo
{
    Q_OBJECT
public:
    CompositorWin7(qt_intf_t *p_intf, QObject* parent = nullptr);

    virtual ~CompositorWin7();

    static bool preInit(qt_intf_t *p_intf);
    virtual bool init() override;

    virtual bool makeMainInterface(MainCtx*) override;
    virtual void destroyMainInterface() override;
    virtual void unloadGUI() override;
    virtual bool setupVoutWindow(vlc_window_t*, VoutDestroyCb destroyCb) override;
    virtual QWindow* interfaceMainWindow() const override;

    Type type() const override;

    QQuickItem * activeFocusItem() const override;

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    int windowEnable(const vlc_window_cfg_t *) override;
    void windowDisable() override;

private slots:
    void resetVideoZOrder();
    void onSurfacePositionChanged(const QPointF& position) override;
    void onSurfaceSizeChanged(const QSizeF& size) override;

private:
    QWidget* m_videoWidget = nullptr;
    QWidget* m_stable = nullptr;
    std::unique_ptr<QQuickView> m_qmlView;
    std::unique_ptr<Win7NativeEventFilter> m_nativeEventFilter;

    HWND m_qmlWindowHWND = nullptr;
    HWND m_videoWindowHWND = nullptr;

};

}


#endif // COMPOSITORDUMMYWIN32_H
