// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef COMPOSITOR_COMMON_HPP
#define COMPOSITOR_COMMON_HPP

#include <QWindow>
#include <QQuickWindow>

namespace vlc {

/**
 * a minimal window with no content
 * this may be useful on linux platform to provide a
 * window which can be drawn into, using a bare QWindow
 * usually freeze on resize
 */
class DummyRenderWindow : public QWindow
{
    Q_OBJECT
public:
    explicit DummyRenderWindow(QWindow* parent = nullptr);

    virtual QAccessibleInterface *accessibleRoot() const override;

protected:
    bool event(QEvent *event) override;

    void resizeEvent(QResizeEvent *resizeEvent) override;

private:
    void init();
    void render();

    QBackingStore* m_backingStore = nullptr;
    bool m_initialized = false;;
};


/**
 * @brief The CompositorOffscreenWindow class allows to fake the visiblilty
 * of the the QQuickWindow, note that this feature will only work if QT5_DECLARATIVE_PRIVATE
 * are available
 */
class CompositorOffscreenWindow : public QQuickWindow
{
    Q_OBJECT
public:
    explicit CompositorOffscreenWindow(QQuickRenderControl* renderControl);

    void setWindowStateExt(Qt::WindowState);

    void setPseudoVisible(bool visible);
};


}

#endif /* COMPOSITOR_COMMON_HPP */
