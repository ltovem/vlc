// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef VLC_COMPOSITOR_DUMMY
#define VLC_COMPOSITOR_DUMMY

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <memory>
#include "compositor.hpp"

class MainCtx;
class QQuickView;
class InterfaceWindowHandler;

namespace vlc {

class CompositorDummy : public QObject, public Compositor
{
    Q_OBJECT
public:
    CompositorDummy(qt_intf_t *p_intf, QObject* parent = nullptr);
    virtual ~CompositorDummy();

    static bool preInit(qt_intf_t*);
    virtual bool init() override;

    virtual bool makeMainInterface(MainCtx*) override;

    /**
     * @brief release all resources used by the compositor.
     * this includes the GUI and the video surfaces.
     */
    virtual void destroyMainInterface() override;

    /**
     * @brief unloadGUI unload the UI view from the composition
     * video view might still be active
     */
    virtual void unloadGUI() override;

    bool setupVoutWindow(vlc_window_t *p_wnd, VoutDestroyCb destroyCb) override;

    QWindow* interfaceMainWindow() const override;

    Type type() const override;

    QQuickItem * activeFocusItem() const override;

protected:
    qt_intf_t *m_intf;

    std::unique_ptr<InterfaceWindowHandler> m_intfWindowHandler;
    MainCtx* m_mainCtx;
    std::unique_ptr<QQuickView> m_qmlWidget;
};

}

#endif // VLC_COMPOSITOR_DUMMY
