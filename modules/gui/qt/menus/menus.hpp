// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * menus.hpp : Menus handling
 *****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *****************************************************************************/

#ifndef QVLC_MENUS_H_
#define QVLC_MENUS_H_

#include "qt.hpp"

#include "custom_menus.hpp"

#include <QObject>

class VLCMenuBar : public QObject
{
    Q_OBJECT
    friend class MenuFunc;

public:
    VLCMenuBar(QObject* parent = nullptr);

    /* Popups Menus */
    static QMenu* PopupMenu( qt_intf_t *, bool );
    static QMenu* AudioPopupMenu( qt_intf_t *, bool );
    static QMenu* VideoPopupMenu( qt_intf_t *, bool );
    static QMenu* MiscPopupMenu( qt_intf_t *, bool );

    /* Systray */
    static void updateSystrayMenu( MainCtx *, qt_intf_t  *,
                                   bool b_force_visible = false);

    /* destructor for parentless Menus (kept in static variables) */
    static void freeRendererMenu(){ delete rendererMenu; rendererMenu = NULL; }

protected:
    /* All main Menus */
    static void FileMenu( qt_intf_t *, QMenu * );

    static void ToolsMenu( qt_intf_t *, QMenu * );

    static void ViewMenu( qt_intf_t *, QMenu *);

    static void InterfacesMenu( qt_intf_t *p_intf, QMenu * );
    static void ExtensionsMenu( qt_intf_t *p_intf, QMenu * );

    static void NavigMenu( qt_intf_t *, QMenu * );

    static void RebuildNavigMenu(qt_intf_t *, QMenu *);

    static void VideoMenu( qt_intf_t *, QMenu * );

    static void SubtitleMenu( qt_intf_t *, QMenu *current, bool b_popup = false );

    static void AudioMenu( qt_intf_t *, QMenu * );

    static void HelpMenu( QMenu *menu );

    /* Popups Menus */
    static void PopupMenuStaticEntries( QMenu *menu );
    static void PopupMenuPlaylistEntries( QMenu *menu, qt_intf_t *p_intf );
    static void PopupMenuPlaylistControlEntries( QMenu *menu, qt_intf_t *p_intf );
    static void PopupMenuControlEntries( QMenu *menu, qt_intf_t *p_intf, bool b = true );

    /* recentMRL menu */
    static RendererMenu *rendererMenu;

    static void updateAudioDevice(qt_intf_t *, QMenu* );
};

#endif
