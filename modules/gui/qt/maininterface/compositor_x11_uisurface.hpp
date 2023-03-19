// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef COMPOSITOR_X11_UISURFACE_HPP
#define COMPOSITOR_X11_UISURFACE_HPP

#include <QWindow>
#include <QQuickWindow>
#include <QQuickRenderControl>
#include "compositor.hpp"

class QQuickWindow;
class QQmlEngine;
class QQmlComponent;
class QQuickItem;
class QQuickRenderControl;

namespace vlc {

class CompositorOffscreenWindow;

class CompositorX11RenderControl : public QQuickRenderControl {
    Q_OBJECT
public:
    CompositorX11RenderControl(QWindow* window, QObject* parent = nullptr)
        : QQuickRenderControl(parent)
        , m_window(window)
    {}


    QWindow *renderWindow(QPoint * offset) override;

private:
    QWindow* m_window = nullptr;
};

class CompositorX11UISurface : public QWindow , public CompositorVideo::QmlUISurface
{
    Q_OBJECT
public:
    explicit CompositorX11UISurface(QWindow* window, QScreen *screen = nullptr);
    ~CompositorX11UISurface();

    virtual void render();

    bool handleWindowEvent(QEvent *event);

    //QmlUISurface API
    void setContent(QQmlComponent*,  QQuickItem* rootItem) override;
    QQmlEngine* engine() const override { return m_qmlEngine; }

    QQuickItem * activeFocusItem() const override;

    QQuickWindow* getOffscreenWindow() const;

signals:
    void beforeRendering();
    void afterRendering();
    void sizeChanged(const QSize& size);
    void updated();

protected:
    bool eventFilter(QObject* object, QEvent *event) override;

    bool event(QEvent *event) override;

    void resizeEvent(QResizeEvent *) override;
    void exposeEvent(QExposeEvent *) override;
    void handleScreenChange();
    void forwardFocusObjectChanged(QObject* focusObject);

    void updateSizes();

    void createFbo();
    void destroyFbo();
    void resizeFbo();

private:
    QQuickItem* m_rootItem = nullptr;
    QOpenGLContext *m_context = nullptr;
    CompositorOffscreenWindow* m_uiWindow = nullptr;
    QQmlEngine* m_qmlEngine = nullptr;
    QWindow* m_renderWindow = nullptr;
    CompositorX11RenderControl* m_uiRenderControl = nullptr;

    QSize m_onscreenSize;
};

}

#endif // COMPOSITOR_X11_UISURFACE_HPP
