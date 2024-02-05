/*****************************************************************************
 * Copyright (C) 2024 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#include "compositor_platform.hpp"

#include <QApplication>
#include <QQuickView>
#include <QOperatingSystemVersion>

#include <vlc_window.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <versionhelpers.h> // Check if Windows8+
#include <dwmapi.h> // To enable native acrylic effect
#endif


using namespace vlc;


CompositorPlatform::CompositorPlatform(qt_intf_t *p_intf, QObject *parent)
    : CompositorVideo(p_intf, parent)
{

}

bool CompositorPlatform::preInit(qt_intf_t *p_intf)
{
    // TODO: For now only qwindows and qdirect2d platforms
    //       running under Windows 8+ are supported.
    //       See if other platforms are also working.
    //       It might be possible to support cocoa,
    //       and wayland.

#ifdef _WIN32
    if (IsWindows8OrGreater())
        return true;
#endif

    return false;
}

bool CompositorPlatform::init()
{
    {
        const QString& platformName = qApp->platformName();
        if (platformName == QLatin1String("windows") || platformName == QLatin1String("direct2d"))
            return true;
    }

    return false;
}

bool CompositorPlatform::makeMainInterface(MainCtx *mainCtx)
{
    m_mainCtx = mainCtx;

    m_rootWindow = std::make_unique<QWindow>();

    m_videoWindow = new QWindow(m_rootWindow.get());

    m_quickWindow = new QQuickView(m_rootWindow.get());
    m_quickWindow->setResizeMode(QQuickView::SizeRootObjectToView);

    {
        // Transparency set-up:
        m_quickWindow->setColor(Qt::transparent);

        QSurfaceFormat format;
        format.setAlphaBufferSize(8);
        format.setDepthBufferSize(16);
        format.setStencilBufferSize(8);
        m_quickWindow->setFormat(format);
    }

    // Make sure the UI child window has the same size as the root parent window:
    connect(m_rootWindow.get(), &QWindow::widthChanged, m_quickWindow, &QWindow::setWidth);
    connect(m_rootWindow.get(), &QWindow::heightChanged, m_quickWindow, &QWindow::setHeight);

    CompositorVideo::Flags flags = CompositorVideo::CAN_SHOW_PIP;

    // If Windows 11 Build 22621, enable acrylic effect:
#ifdef _WIN32
    if (QOperatingSystemVersion::current()
        >= QOperatingSystemVersion(QOperatingSystemVersion::Windows, 11, 0, 22621))
    {
        flags |= CompositorVideo::HAS_ACRYLIC;
    }
#endif

    const bool ret = commonGUICreate(m_rootWindow.get(), m_quickWindow, flags);

    m_quickWindow->create();

    m_quickWindow->setFlag(Qt::FramelessWindowHint);
    // Qt QPA Bug (qwindows, qdirect2d(?)): to trigger WS_EX_LAYERED set up.
    m_quickWindow->setOpacity(0.0);
    m_quickWindow->setOpacity(1.0);

#ifdef _WIN32
    if (flags | CompositorVideo::HAS_ACRYLIC)
    {
        enum BackdropType
        {
            DWMSBT_TRANSIENTWINDOW = 3
        } backdropType = DWMSBT_TRANSIENTWINDOW;
        DwmSetWindowAttribute(reinterpret_cast<HWND>(m_quickWindow->winId()),
                              38 /* DWMWA_SYSTEMBACKDROP_TYPE */,
                              &backdropType,
                              sizeof(backdropType));
    }
#endif

    m_rootWindow->show();
    m_videoWindow->show();
    m_quickWindow->show();

    m_quickWindow->raise(); // Make sure quick window is above the video window.

    return ret;
}

void CompositorPlatform::destroyMainInterface()
{
    commonIntfDestroy();
}

void CompositorPlatform::unloadGUI()
{
    commonGUIDestroy();
}

bool CompositorPlatform::setupVoutWindow(vlc_window_t *p_wnd, VoutDestroyCb destroyCb)
{
    commonSetupVoutWindow(p_wnd, destroyCb);

    assert(qApp->platformName() == QLatin1String("windows") || qApp->platformName() == QLatin1String("direct2d"));

    p_wnd->type = VLC_WINDOW_TYPE_HWND;
    p_wnd->handle.hwnd = reinterpret_cast<void*>(m_videoWindow->winId());

    return true;
}

QWindow *CompositorPlatform::interfaceMainWindow() const
{
    return m_rootWindow.get();
}

Compositor::Type CompositorPlatform::type() const
{
    return Compositor::PlatformCompositor;
}

QQuickItem *CompositorPlatform::activeFocusItem() const
{
    assert(m_quickWindow);
    return m_quickWindow->activeFocusItem();
}

int CompositorPlatform::windowEnable(const vlc_window_cfg_t *)
{
    commonWindowEnable();
    return VLC_SUCCESS;
}

void CompositorPlatform::windowDisable()
{
    commonWindowDisable();
}

void CompositorPlatform::onSurfacePositionChanged(const QPointF &position)
{
    m_videoWindow->setPosition((position / m_videoWindow->devicePixelRatio()).toPoint());
}

void CompositorPlatform::onSurfaceSizeChanged(const QSizeF &size)
{
    m_videoWindow->resize((size / m_videoWindow->devicePixelRatio()).toSize());
}
