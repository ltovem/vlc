/*****************************************************************************
 * menus.hpp : Menus handling
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
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

#ifndef QVLC_MENUS_H_
#define QVLC_MENUS_H_

#include "qt.hpp"

#include "custom_menus.hpp"
#include "widgets/native/platformagnosticmenu.hpp"
#include "widgets/native/platformagnosticaction.hpp"
#include "widgets/native/platformagnosticactiongroup.hpp"

#include <QObject>

template <class Menu>
struct Types{ };

template<>
struct Types<QMenu>
{
    using actionType = QAction;
    using iconType = QIcon;
};

template <>
struct Types<PlatformAgnosticMenu>
{
    using actionType = PlatformAgnosticAction;
    using iconType = QString;
};

class VLCMenuBar : public QObject
{
    Q_OBJECT
    friend class MenuFunc;

public:
    VLCMenuBar(QObject* parent = nullptr);

    /* Popups Menus */
    static PlatformAgnosticMenu* PopupMenu( qt_intf_t *, bool );
    static PlatformAgnosticMenu* AudioPopupMenu( qt_intf_t *, bool );
    static PlatformAgnosticMenu* VideoPopupMenu( qt_intf_t *, bool );
    static PlatformAgnosticMenu* MiscPopupMenu( qt_intf_t *, bool );

    /* Systray */
    static void updateSystrayMenu( MainCtx *, qt_intf_t  *,
                                   bool b_force_visible = false);

    /* destructor for parentless Menus (kept in static variables) */
    static void freeRendererMenu(){ delete rendererMenu; rendererMenu = NULL; }

protected:
    /* All main Menus */

    template<class Menu, class Action = typename Types<Menu>::actionType>
    static void FileMenu( qt_intf_t *, Menu * );

    template<class Menu>
    static void ToolsMenu( qt_intf_t *, Menu * );

    template<class Menu, class Action = typename Types<Menu>::actionType, class Icon = typename Types<Menu>::iconType>
    static void ViewMenu( qt_intf_t *, Menu *);

    template<class Menu>
    static void InterfacesMenu( qt_intf_t *p_intf, Menu * );

    template<class Menu>
    static void ExtensionsMenu( qt_intf_t *p_intf, Menu * );

    template<class Menu, class Action = typename Types<Menu>::actionType>
    static void NavigMenu( qt_intf_t *, Menu * );

    template<class Menu, class Action = typename Types<Menu>::actionType>
    static void RebuildNavigMenu(qt_intf_t *, Menu *);

    template<class Menu, class Action = typename Types<Menu>::actionType>
    static void VideoMenu( qt_intf_t *, Menu * );

    template<class Menu>
    static void SubtitleMenu( qt_intf_t *, Menu *current, bool b_popup = false );

    template<class Menu, class Action = typename Types<Menu>::actionType>
    static void AudioMenu( qt_intf_t *, Menu * );

    template<class Menu>
    static void HelpMenu( Menu *menu );

    /* Popups Menus */
    template<class Menu, class Action = typename Types<Menu>::actionType>
    static void PopupMenuStaticEntries( Menu *menu );
    template<class Menu, class Action = typename Types<Menu>::actionType, class Icon = typename Types<Menu>::iconType>
    static void PopupMenuPlaylistEntries( Menu *menu, qt_intf_t *p_intf );
    template<class Menu>
    static void PopupMenuPlaylistControlEntries( Menu *menu, qt_intf_t *p_intf );
    template<class Menu, class Action = typename Types<Menu>::actionType, class Icon = typename Types<Menu>::iconType>
    static void PopupMenuControlEntries( Menu *menu, qt_intf_t *p_intf, bool b = true );

    /* recentMRL menu */
    static RendererMenu *rendererMenu;

    template<class ActionGroup, class Menu, class Action = typename Types<Menu>::actionType>
    static void updateAudioDevice(qt_intf_t *, Menu* );
};

#endif
