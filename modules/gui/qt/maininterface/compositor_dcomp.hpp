// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef VLC_COMPOSITOR_DIRECT_COMPOSITION
#define VLC_COMPOSITOR_DIRECT_COMPOSITION

#include "compositor.hpp"

#include <windows.h>

#include "maininterface/mainui.hpp"
#include "compositor_dcomp_acrylicsurface.hpp"
#include "compositor_dcomp_uisurface.hpp"
#include "videosurface.hpp"
#include "interface_window_handler.hpp"
#include "video_window_handler.hpp"

#include <QOpenGLContext>

class MainCtx;
class WinTaskbarWidget;

namespace vlc {

class CompositorDirectComposition : public CompositorVideo
{
    Q_OBJECT
public:
    CompositorDirectComposition(qt_intf_t *p_intf, QObject* parent = nullptr);
    ~CompositorDirectComposition();

    static bool preInit(qt_intf_t *);
    bool init() override;

    bool makeMainInterface(MainCtx*) override;
    void destroyMainInterface() override;
    void unloadGUI() override;

    bool setupVoutWindow(vlc_window_t *p_wnd, VoutDestroyCb destroyCb) override;
    virtual QWindow* interfaceMainWindow() const override;

    Type type() const override;

    void addVisual(Microsoft::WRL::ComPtr<IDCompositionVisual> visual);
    void removeVisual(Microsoft::WRL::ComPtr<IDCompositionVisual> visual);

    QQuickItem * activeFocusItem() const override;

private slots:
    void onSurfacePositionChanged(const QPointF& position) override;
    void onSurfaceSizeChanged(const QSizeF& size) override;

protected:
    int windowEnable(const vlc_window_cfg_t *) override;
    void windowDisable() override;
    void windowDestroy() override;

private:
    DCompRenderWindow* m_rootWindow = nullptr;

    std::unique_ptr<WinTaskbarWidget> m_taskbarWidget;

    std::unique_ptr<CompositorDCompositionUISurface> m_uiSurface;
    std::unique_ptr<CompositorDCompositionAcrylicSurface> m_acrylicSurface;

    //main window composition
    HINSTANCE m_dcomp_dll = nullptr;
    Microsoft::WRL::ComPtr<ID3D11Device> m_d3d11Device;
    Microsoft::WRL::ComPtr<IDCompositionDevice> m_dcompDevice;
    Microsoft::WRL::ComPtr<IDCompositionTarget> m_dcompTarget;
    Microsoft::WRL::ComPtr<IDCompositionVisual> m_rootVisual;
    Microsoft::WRL::ComPtr<IDCompositionVisual> m_uiVisual;
    Microsoft::WRL::ComPtr<IDCompositionVisual> m_videoVisual;
};

}

#endif /* VLC_COMPOSITOR_DIRECT_COMPOSITION */
