/*****************************************************************************
 * menus.cpp : Qt menus
 *****************************************************************************
 * Copyright © 2006-2011 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Jean-Philippe André <jpeg@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
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

/** \todo
 * - Remove static currentGroup
 */

#include "qstringliteral.h"
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_intf_strings.h>
#include <vlc_aout.h>                             /* audio_output_t */
#include <vlc_interface.h>

#include "menus.hpp"

#include "maininterface/mainctx.hpp"                     /* View modifications */
#include "dialogs/dialogs_provider.hpp"                   /* Dialogs display */
#include "player/player_controller.hpp"                      /* Input Management */
#include "playlist/playlist_controller.hpp"
#include "dialogs/extensions/extensions_manager.hpp"                 /* Extensions menu */
#include "dialogs/extended/extended_panels.hpp"
#include "util/varchoicemodel.hpp"
#include "medialibrary/medialib.hpp"
#include "medialibrary/mlrecentsmodel.hpp"
#include "medialibrary/mlbookmarkmodel.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QSignalMapper>

using namespace vlc::playlist;

/*
  This file defines the main menus and the pop-up menu (right-click menu)
  and the systray menu (in that order in the file)

  There are 4 menus that have to be rebuilt every time there are called:
  Audio, Video, Navigation, view
  4 functions are building those menus: AudioMenu, VideoMenu, NavigMenu, View

  */

RendererMenu *VLCMenuBar::rendererMenu = NULL;

/**
 * @brief Add static entries to DP in menus
 **/
template<typename Functor, class Menu, class Action = typename Types<Menu>::actionType, class Icon = typename Types<Menu>::iconType>
static Action *addDPStaticEntry( Menu *menu,
                       const QString& text,
                       const QString& icon,
                       const Functor member,
                       const char *shortcut = NULL,
                       typename Action::MenuRole role = Action::NoRole
                       )
{
    Action *action = NULL;
#ifndef __APPLE__ /* We don't set icons in menus in MacOS X */
    if( !icon.isEmpty() )
    {
        if( !EMPTY_STR( shortcut ) )
            action = menu->addAction( Icon( icon ), text, THEDP,
                                      member, qfut( shortcut ) );
        else
            action = menu->addAction( Icon( icon ), text, THEDP, member );
    }
    else
#endif
    {
        if( !EMPTY_STR( shortcut ) )
            action = menu->addAction( text, THEDP, member, qfut( shortcut ) );
        else
            action = menu->addAction( text, THEDP, member );
    }
#ifdef __APPLE__
    if ( qobject_cast<WidgetsAction*>( action ) )
        action->setMenuRole( role );
#else
    Q_UNUSED( role );
#endif
    return action;
}

/**
 * @brief Add static entries to MIM in menus
 **/
template<typename Fun, class Menu, class Action = typename Types<Menu>::actionType, class Icon = typename Types<Menu>::iconType>
static Action* addMIMStaticEntry( qt_intf_t *p_intf,
                            Menu *menu,
                            const QString& text,
                            const QString& icon,
                            Fun member )
{
    Action *action;
#ifndef __APPLE__ /* We don't set icons in menus in MacOS X */
    if( !icon.isEmpty() )
    {
        action = menu->addAction( text, THEMIM,  member );
        action->setIcon( Icon( icon ) );
    }
    else
#endif
    {
        action = menu->addAction( text, THEMIM, member );
    }
    return action;
}

template<typename Fun, class Menu, class Action = typename Types<Menu>::actionType, class Icon = typename Types<Menu>::iconType>
static Action* addMPLStaticEntry( qt_intf_t *p_intf,
                            Menu *menu,
                            const QString& text,
                            const QString& icon,
                            Fun member )
{
    Action *action;
#ifndef __APPLE__ /* We don't set icons in menus in MacOS X */
    if( !icon.isEmpty() )
    {
        action = menu->addAction( text, THEMPL,  member );
        action->setIcon( Icon( icon ) );
    }
    else
#endif
    {
        action = menu->addAction( text, THEMPL, member );
    }
    return action;
}

/*****************************************************************************
 * All normal menus
 * Simple Code
 *****************************************************************************/

/**
 * Main Menu Bar Creation
 **/
VLCMenuBar::VLCMenuBar(QObject* parent)
    : QObject(parent)
{}


/**
 * Media ( File ) Menu
 * Opening, streaming and quit
 **/
template<class Menu, class Action>
void VLCMenuBar::FileMenu(qt_intf_t *p_intf, Menu *menu)
{
    PlatformAgnosticMenu* const platformAgnosticMenu = PlatformAgnosticMenu::fromMenu(menu);

    auto mi = p_intf->p_mi;
    Action *action;

    //use a lambda here as the Triggrered signal is emiting and it will pass false (checked) as a first argument
    addDPStaticEntry( platformAgnosticMenu, qtr( "Open &File..." ),
        QStringLiteral(":/menu/file.svg"), []() { THEDP->simpleOpenDialog(); } , "Ctrl+O" );
    addDPStaticEntry( platformAgnosticMenu, qtr( "&Open Multiple Files..." ),
        QStringLiteral(":/menu/file.svg"), &DialogsProvider::openFileDialog, "Ctrl+Shift+O" );
    addDPStaticEntry( platformAgnosticMenu, qfut( I_OP_OPDIR ),
        QStringLiteral(":/menu/folder.svg"), &DialogsProvider::PLOpenDir, "Ctrl+F" );
    addDPStaticEntry( platformAgnosticMenu, qtr( "Open &Disc..." ),
        QStringLiteral(":/menu/disc.svg"), &DialogsProvider::openDiscDialog, "Ctrl+D" );
    addDPStaticEntry( platformAgnosticMenu, qtr( "Open &Network Stream..." ),
        QStringLiteral(":/menu/network.svg"), &DialogsProvider::openNetDialog, "Ctrl+N" );
    addDPStaticEntry( platformAgnosticMenu, qtr( "Open &Capture Device..." ),
        QStringLiteral(":/menu/capture-card.svg"), &DialogsProvider::openCaptureDialog, "Ctrl+C" );

    addDPStaticEntry( platformAgnosticMenu, qtr( "Open &Location from clipboard" ),
                      NULL, &DialogsProvider::openUrlDialog, "Ctrl+V" );

    if( mi && var_InheritBool( p_intf, "save-recentplay" ) && mi->hasMediaLibrary() )
    {
        MLRecentsModel* recentModel = new MLRecentsModel(nullptr);
        recentModel->setMl(mi->getMediaLibrary());
        recentModel->setLimit(10);
        Menu* recentsMenu = (*new RecentMenu(recentModel, mi->getMediaLibrary(), platformAgnosticMenu))();
        recentsMenu->setTitle(qtr( "Open &Recent Media" ) );
        recentModel->setParent(recentsMenu);
        menu->addMenu( recentsMenu );
    }

    platformAgnosticMenu->addSeparator();

    addDPStaticEntry( platformAgnosticMenu, qfut( I_PL_SAVE ), "", &DialogsProvider::savePlayingToPlaylist,
        "Ctrl+Y" );

    addDPStaticEntry( platformAgnosticMenu, qtr( "Conve&rt / Save..." ), "",
        &DialogsProvider::openAndTranscodingDialogs, "Ctrl+R" );
    addDPStaticEntry( platformAgnosticMenu, qtr( "&Stream..." ),
        QStringLiteral(":/menu/stream.svg"), &DialogsProvider::openAndStreamingDialogs, "Ctrl+S" );
    platformAgnosticMenu->addSeparator();

    action = addMPLStaticEntry( p_intf, platformAgnosticMenu, qtr( "Quit at the end of playlist" ), "",
                               &PlaylistController::playAndExitChanged );
    action->setCheckable( true );
    action->setChecked( THEMPL->isPlayAndExit() );

    if( mi && mi->getSysTray() )
    {
        action = menu->addAction( qtr( "Close to systray"), mi,
                                &MainCtx::toggleUpdateSystrayMenu );
    }

    addDPStaticEntry( platformAgnosticMenu, qtr( "&Quit" ) ,
        QStringLiteral(":/menu/exit.svg"), &DialogsProvider::quit, "Ctrl+Q" );
}

/**
 * Tools, like Media Information, Preferences or Messages
 **/
template<class Menu>
void VLCMenuBar::ToolsMenu( qt_intf_t *p_intf, Menu *menu )
{
    addDPStaticEntry( menu, qtr( "&Effects and Filters"), QStringLiteral(":/menu/ic_fluent_options.svg"),
        &DialogsProvider::extendedDialog, "Ctrl+E" );

    addDPStaticEntry( menu, qtr( "&Track Synchronization"), QStringLiteral(":/menu/ic_fluent_options.svg"),
        &DialogsProvider::synchroDialog, "" );

    addDPStaticEntry( menu, qfut( I_MENU_INFO ) , QStringLiteral(":/menu/info.svg"),
        QOverload<>::of(&DialogsProvider::mediaInfoDialog), "Ctrl+I" );

    addDPStaticEntry( menu, qfut( I_MENU_CODECINFO ) ,
        QStringLiteral(":/menu/info.svg"), &DialogsProvider::mediaCodecDialog, "Ctrl+J" );

#ifdef ENABLE_VLM
    addDPStaticEntry( menu, qfut( I_MENU_VLM ), "", &DialogsProvider::vlmDialog,
        "Ctrl+Shift+W" );
#endif

    addDPStaticEntry( menu, qtr( "Program Guide" ), QStringLiteral(""), &DialogsProvider::epgDialog,
        "" );

    addDPStaticEntry( menu, qfut( I_MENU_MSG ),
        QStringLiteral(":/menu/messages.svg"), &DialogsProvider::messagesDialog, "Ctrl+M" );

    addDPStaticEntry( menu, qtr( "Plu&gins and extensions" ),
        QStringLiteral(""), &DialogsProvider::pluginDialog );
    menu->addSeparator();

    if( !p_intf->b_isDialogProvider )
        addDPStaticEntry( menu, qtr( "Customi&ze Interface..." ),
             QStringLiteral(":/menu/preferences.svg"), &DialogsProvider::showToolbarEditorDialog);

    addDPStaticEntry( menu, qtr( "&Preferences" ),
        QStringLiteral(":/menu/preferences.svg"), &DialogsProvider::prefsDialog, "Ctrl+P", PlatformAgnosticAction::PreferencesRole );
}

/**
 * View Menu
 * Interface modification, load other interfaces, activate Extensions
 * \param current, set to NULL for menu creation, else for menu update
 **/
template<class Menu, class Action, class Icon>
void VLCMenuBar::ViewMenu( qt_intf_t *p_intf, Menu *menu )
{
    assert(menu);
    PlatformAgnosticMenu* const platformAgnosticMenu = PlatformAgnosticMenu::fromMenu(menu);

    Action *action;

    MainCtx *mi = p_intf->p_mi;
    assert( mi );

    //menu->clear();
    //HACK menu->clear() does not delete submenus
    QList<PlatformAgnosticAction*> actions = platformAgnosticMenu->actions();
    foreach( PlatformAgnosticAction *a, actions )
    {
        PlatformAgnosticMenu *m = static_cast<PlatformAgnosticMenu*>(a->parent());
        if( a->parent() == platformAgnosticMenu ) delete a;
        else platformAgnosticMenu->removeAction( a );
        if( m && m->parent() == platformAgnosticMenu ) delete m;
    }

    QString title;

    if (mi->hasMediaLibrary())
        title = qtr("Media Library");
    else
        title = qtr("Browse and Discover");

    action = platformAgnosticMenu->addAction(
#ifndef __APPLE__
        Icon( QStringLiteral(":/menu/media_library.svg") ),
#endif
            title);
    action->setCheckable( true );
    connect( action, &Action::triggered, mi, &MainCtx::setMediaLibraryVisible );
    action->setChecked( mi->isMediaLibraryVisible() );

    action = platformAgnosticMenu->addAction(
#ifndef __APPLE__
        Icon( QStringLiteral(":/menu/ic_playlist.svg") ),
#endif
            qtr( "Play&list" ));
    action->setShortcut(QString( "Ctrl+L" ));
    action->setCheckable( true );
    connect( action, &Action::triggered, mi, &MainCtx::setPlaylistVisible );
    action->setChecked( mi->isPlaylistVisible() );

    /* Docked Playlist */
    action = platformAgnosticMenu->addAction( qtr( "Docked Playlist" ) );
    action->setCheckable( true );
    connect( action, &Action::triggered, mi, &MainCtx::setPlaylistDocked );
    action->setChecked( mi->isPlaylistDocked() );

    platformAgnosticMenu->addSeparator();

    action = platformAgnosticMenu->addAction( qtr( "Always on &top" ) );
    action->setCheckable( true );
    action->setChecked( mi->isInterfaceAlwaysOnTop() );
    connect( action, &Action::triggered, mi, &MainCtx::setInterfaceAlwaysOnTop );

    platformAgnosticMenu->addSeparator();

    /* FullScreen View */
    action = platformAgnosticMenu->addAction( qtr( "&Fullscreen Interface" ), mi,
            &MainCtx::toggleInterfaceFullScreen, QString( "F11" ) );
    action->setCheckable( true );
    action->setChecked( mi->isInterfaceFullScreen() );

    action = platformAgnosticMenu->addAction( qtr( "&View Items as Grid" ), mi,
            &MainCtx::setGridView );
    action->setCheckable( true );
    action->setChecked( mi->hasGridView() );

    platformAgnosticMenu->addMenu( (*new CheckableListMenu(qtr( "&Color Scheme" ), mi->getColorScheme(), CheckableListMenu::GROUPED_EXLUSIVE, platformAgnosticMenu))() );

    platformAgnosticMenu->addSeparator();

    InterfacesMenu( p_intf, platformAgnosticMenu );

    /* Extensions */
    ExtensionsMenu( p_intf, platformAgnosticMenu );
}

/**
 * Interface Sub-Menu, to list extras interface and skins
 **/
template<class Menu>
void VLCMenuBar::InterfacesMenu( qt_intf_t *p_intf, Menu *current )
{
    assert(current);
    PlatformAgnosticMenu* const platformAgnosticMenu = PlatformAgnosticMenu::fromMenu(current);

    VLCVarChoiceModel* model = new VLCVarChoiceModel(VLC_OBJECT(p_intf->intf), "intf-add", platformAgnosticMenu);
    const auto submenu = (*new CheckableListMenu(qtr("Interfaces"), model, CheckableListMenu::UNGROUPED, platformAgnosticMenu))();
    platformAgnosticMenu->addMenu(submenu);
}

/**
 * Extensions menu: populate the current menu with extensions
 **/
template<class Menu>
void VLCMenuBar::ExtensionsMenu( qt_intf_t *p_intf, Menu *extMenu )
{
    /* Get ExtensionsManager and load extensions if needed */
    ExtensionsManager *extMgr = ExtensionsManager::getInstance( p_intf );

    if( !var_InheritBool( p_intf, "qt-autoload-extensions")
        && !extMgr->isLoaded() )
    {
        return;
    }

    if( !extMgr->isLoaded() && !extMgr->cannotLoad() )
    {
        extMgr->loadExtensions();
    }

    /* Let the ExtensionsManager build itself the menu */
    extMenu->addSeparator();
    extMgr->menu( extMenu );
}

template<class Menu, class Icon = typename Types<Menu>::iconType>
static inline void VolumeEntries( qt_intf_t *p_intf, Menu *current )
{
    current->addSeparator();

    current->addAction( Icon( QStringLiteral(":/menu/volume-high.svg") ), qtr( "&Increase Volume" ), THEMIM, &PlayerController::setVolumeUp );
    current->addAction( Icon( QStringLiteral(":/menu/volume-low.svg") ), qtr( "&Decrease Volume" ), THEMIM, &PlayerController::setVolumeDown );
    current->addAction( Icon( QStringLiteral(":/menu/volume-muted.svg") ), qtr( "&Mute" ), THEMIM, &PlayerController::toggleMuted );
}

/**
 * Main Audio Menu
 **/
template<class Menu, class Action>
void VLCMenuBar::AudioMenu( qt_intf_t *p_intf, Menu * current )
{
    if( current->isEmpty() )
    {
        PlatformAgnosticMenu* const platformAgnosticMenu = PlatformAgnosticMenu::fromMenu(current);

        platformAgnosticMenu->addMenu((*new CheckableListMenu(qtr( "Audio &Track" ), THEMIM->getAudioTracks(), CheckableListMenu::GROUPED_OPTIONAL, platformAgnosticMenu))());

        PlatformAgnosticMenu *const audioDeviceSubmenu = platformAgnosticMenu->addMenu(qtr( "&Audio Device" ));
        connect(audioDeviceSubmenu, &PlatformAgnosticMenu::aboutToShow, [=]() {
            updateAudioDevice<PlatformAgnosticActionGroup>( p_intf, audioDeviceSubmenu );
        });

        VLCVarChoiceModel *mix_mode = THEMIM->getAudioMixMode();

        if (mix_mode->rowCount() == 0)
            platformAgnosticMenu->addMenu( (*new CheckableListMenu(qtr( "&Stereo Mode" ), THEMIM->getAudioStereoMode(), CheckableListMenu::GROUPED_EXLUSIVE, platformAgnosticMenu))() );
        else
            platformAgnosticMenu->addMenu( (*new CheckableListMenu(qtr( "&Mix Mode" ), mix_mode, CheckableListMenu::GROUPED_EXLUSIVE, platformAgnosticMenu))() );
        platformAgnosticMenu->addSeparator();

        platformAgnosticMenu->addMenu( (*new CheckableListMenu(qtr( "&Visualizations" ), THEMIM->getAudioVisualizations(), CheckableListMenu::GROUPED_EXLUSIVE, platformAgnosticMenu))() );
        VolumeEntries( p_intf, platformAgnosticMenu );
    }
}

/* Subtitles */
template<class Menu>
void VLCMenuBar::SubtitleMenu( qt_intf_t *p_intf, Menu *current, bool b_popup )
{
    if( current->isEmpty() || b_popup )
    {
        PlatformAgnosticMenu* const platformAgnosticMenu = PlatformAgnosticMenu::fromMenu(current);

        addDPStaticEntry( platformAgnosticMenu, qtr( "Add &Subtitle File..." ), "",
                &DialogsProvider::loadSubtitlesFile);
        platformAgnosticMenu->addMenu((*new CheckableListMenu(qtr( "Sub &Track" ), THEMIM->getSubtitleTracks(), CheckableListMenu::GROUPED_OPTIONAL, platformAgnosticMenu))());
        platformAgnosticMenu->addSeparator();
    }
}

/**
 * Main Video Menu
 * Subtitles are part of Video.
 **/
template<class Menu, class Action>
void VLCMenuBar::VideoMenu( qt_intf_t *p_intf, Menu *current )
{
    if( current->isEmpty() )
    {
        PlatformAgnosticMenu *const menu = PlatformAgnosticMenu::fromMenu(current);

        menu->addMenu((*new CheckableListMenu(qtr( "Video &Track" ), THEMIM->getVideoTracks(), CheckableListMenu::GROUPED_OPTIONAL, menu))());

        menu->addSeparator();
        /* Surface modifiers */
        menu->addAction((*new BooleanPropertyAction(qtr( "&Fullscreen"), THEMIM, "fullscreen", menu))());
        PlatformAgnosticAction* action = (*new BooleanPropertyAction(qtr( "Always Fit &Window"), THEMIM, "autoscale", menu))();
        action->setEnabled( THEMIM->hasVideoOutput() );
        connect(THEMIM, &PlayerController::hasVideoOutputChanged, action, &PlatformAgnosticAction::setEnabled);
        menu->addAction(action);
        menu->addAction((*new BooleanPropertyAction(qtr( "Set as Wall&paper"), THEMIM, "wallpaperMode", menu))());

        menu->addSeparator();
        /* Size modifiers */
        menu->addMenu( (*new CheckableListMenu(qtr( "&Zoom" ), THEMIM->getZoom(), CheckableListMenu::GROUPED_EXLUSIVE, menu))() );
        menu->addMenu( (*new CheckableListMenu(qtr( "&Aspect Ratio" ), THEMIM->getAspectRatio(), CheckableListMenu::GROUPED_EXLUSIVE, menu))() );
        menu->addMenu( (*new CheckableListMenu(qtr( "&Crop" ), THEMIM->getCrop(), CheckableListMenu::GROUPED_EXLUSIVE, menu))() );

        menu->addSeparator();
        /* Rendering modifiers */
        menu->addMenu( (*new CheckableListMenu(qtr( "&Deinterlace" ), THEMIM->getDeinterlace(), CheckableListMenu::GROUPED_EXLUSIVE, menu))() );
        menu->addMenu( (*new CheckableListMenu(qtr( "&Deinterlace mode" ), THEMIM->getDeinterlaceMode(), CheckableListMenu::GROUPED_EXLUSIVE, menu))() );

        menu->addSeparator();
        /* Other actions */
        PlatformAgnosticAction* snapshotAction = PlatformAgnosticAction::createAction(qtr( "Take &Snapshot" ), menu);
        connect(snapshotAction, &PlatformAgnosticAction::triggered, THEMIM, &PlayerController::snapshot);
        menu->addAction(snapshotAction);
    }
}

/**
 * Navigation Menu
 * For DVD, MP4, MOV and other chapter based format
 **/
template<class Menu, class Action>
void VLCMenuBar::NavigMenu( qt_intf_t *p_intf, Menu *menu )
{
    PlatformAgnosticMenu *const platformAgnosticMenu = PlatformAgnosticMenu::fromMenu(menu);

    PlatformAgnosticAction *action;
    PlatformAgnosticMenu *submenu;

    platformAgnosticMenu->addMenu( (*new CheckableListMenu( qtr( "T&itle" ), THEMIM->getTitles(), CheckableListMenu::GROUPED_EXLUSIVE, platformAgnosticMenu))() );
    submenu = (*new CheckableListMenu( qtr( "&Chapter" ), THEMIM->getChapters(), CheckableListMenu::GROUPED_EXLUSIVE, platformAgnosticMenu ))();
    if (qobject_cast<WidgetsMenu*>(submenu))
        submenu->setTearOffEnabled( true );
    platformAgnosticMenu->addMenu( submenu );
    platformAgnosticMenu->addMenu( (*new CheckableListMenu( qtr("&Program") , THEMIM->getPrograms(), CheckableListMenu::GROUPED_EXLUSIVE, platformAgnosticMenu))() );

    MainCtx * mi = p_intf->p_mi;

    if (mi && p_intf->p_mi->hasMediaLibrary() )
    {
        MediaLib * mediaLib = mi->getMediaLibrary();

        BookmarkMenu * bookmarks = new BookmarkMenu(mediaLib, p_intf->p_player, menu);

        (*bookmarks)()->setTitle(qfut(I_MENU_BOOKMARK));

        platformAgnosticMenu->addMenu((*bookmarks)());
        // action->setData("bookmark"); // TODO
    }

    platformAgnosticMenu->addSeparator();

    if ( !VLCMenuBar::rendererMenu )
        VLCMenuBar::rendererMenu = new RendererMenu( p_intf, platformAgnosticMenu );

    VLCMenuBar::rendererMenu->setParent(nullptr); // This is a parentless menu
    VLCMenuBar::rendererMenu->m_menu->setParent(nullptr);

    menu->addMenu( VLCMenuBar::rendererMenu->m_menu );
    platformAgnosticMenu->addSeparator();

    PopupMenuControlEntries( platformAgnosticMenu, p_intf );

    RebuildNavigMenu( p_intf, platformAgnosticMenu );
}

template<class Menu, class Action>
void VLCMenuBar::RebuildNavigMenu( qt_intf_t *p_intf, Menu *menu )
{
    Action* action;

#define ADD_ACTION( title, slot, visibleSignal, visible ) \
    action = menu->addAction( title, THEMIM,  slot ); \
    //if (! visible)\
    //    action->setVisible(false); \
    //connect( THEMIM, visibleSignal, action, &Action::setVisible );

    ADD_ACTION( qtr("Previous Title"), &PlayerController::titlePrev, &PlayerController::hasTitlesChanged, THEMIM->hasTitles() );
    ADD_ACTION( qtr("Next Title"), &PlayerController::titleNext, &PlayerController::hasTitlesChanged, THEMIM->hasTitles() );
    ADD_ACTION( qtr("Previous Chapter"), &PlayerController::chapterPrev, &PlayerController::hasChaptersChanged, THEMIM->hasChapters() );
    ADD_ACTION( qtr("Next Chapter"), &PlayerController::chapterNext, &PlayerController::hasChaptersChanged, THEMIM->hasChapters() );

#undef ADD_ACTION

    PopupMenuPlaylistEntries( menu, p_intf );
}

/**
 * Help/About Menu
**/
template<class Menu>
void VLCMenuBar::HelpMenu( Menu *menu )
{
    addDPStaticEntry( menu, qtr( "&Help" ) ,
        QStringLiteral(":/menu/help.svg"), &DialogsProvider::helpDialog, "F1" );
#ifdef UPDATE_CHECK
    addDPStaticEntry( menu, qtr( "Check for &Updates..." ) , "",
                      &DialogsProvider::updateDialog);
#endif
    menu->addSeparator();
    addDPStaticEntry( menu, qfut( I_MENU_ABOUT ), QStringLiteral(":/menu/info.svg"),
            &DialogsProvider::aboutDialog, "Shift+F1", PlatformAgnosticAction::AboutRole );
}

/*****************************************************************************
 * Popup menus - Right Click menus                                           *
 *****************************************************************************/

template<class Menu, class Action, class Icon>
void VLCMenuBar::PopupMenuPlaylistEntries( Menu *menu, qt_intf_t *p_intf )
{
    Action *action;
    bool hasInput = THEMIM->hasInput();

    /* Play or Pause action and icon */
    if( !hasInput || THEMIM->getPlayingState() != PlayerController::PLAYING_STATE_PLAYING )
    {
        action = menu->addAction( qtr( "&Play" ), [p_intf]() {
            if ( THEMPL->isEmpty() )
                THEDP->openFileDialog();
            else
                THEMPL->togglePlayPause();
        });
#ifndef __APPLE__ /* No icons in menus in Mac */
        action->setIcon( Icon{ QStringLiteral(":/menu/ic_fluent_play_filled.svg") } );
#endif
    }
    else
    {
        action = addMPLStaticEntry( p_intf, menu, qtr( "Pause" ),
                    QStringLiteral(":/menu/ic_pause_filled.svg"), &PlaylistController::togglePlayPause );
    }

    /* Stop */
    action = addMPLStaticEntry( p_intf, menu, qtr( "&Stop" ),
                QStringLiteral(":/menu/ic_fluent_stop.svg"), &PlaylistController::stop );
    if( !hasInput )
        action->setEnabled( false );

    /* Next / Previous */
    bool bPlaylistEmpty = THEMPL->isEmpty();
    Action* previousAction = addMPLStaticEntry( p_intf, menu, qtr( "Pre&vious" ),
                                QStringLiteral(":/menu/ic_fluent_previous.svg"), &PlaylistController::prev );
    previousAction->setEnabled( !bPlaylistEmpty );
    connect( THEMPL, &PlaylistController::isEmptyChanged, previousAction, [previousAction]() {
        previousAction->setEnabled(false);
    });

    Action* nextAction = addMPLStaticEntry( p_intf, menu, qtr( "Ne&xt" ),
                            QStringLiteral(":/menu/ic_fluent_next.svg"), &PlaylistController::next );
    nextAction->setEnabled( !bPlaylistEmpty );
    connect( THEMPL, &PlaylistController::isEmptyChanged, nextAction, [nextAction]() {
        nextAction->setEnabled(false);
    });

    action = menu->addAction( qtr( "Record" ), THEMIM, &PlayerController::toggleRecord );
    action->setIcon( Icon( QStringLiteral(":/menu/record.svg") ) );
    if( !hasInput )
        action->setEnabled( false );
    menu->addSeparator();
}

template<class Menu , class Action, class Icon>
void VLCMenuBar::PopupMenuControlEntries( Menu *menu, qt_intf_t *p_intf,
                                        bool b_normal )
{
    PlatformAgnosticMenu *const platformAgnosticMenu = PlatformAgnosticMenu::fromMenu(menu);

    PlatformAgnosticAction *action;
    PlatformAgnosticMenu *rateMenu = PlatformAgnosticMenu::createMenu( qtr( "Sp&eed" ), platformAgnosticMenu );
    if (qobject_cast<WidgetsMenu*>(rateMenu))
        rateMenu->setTearOffEnabled( true );

    if( b_normal )
    {
        /* Faster/Slower */
        action = rateMenu->addAction( qtr( "&Faster" ), THEMIM,
                                  &PlayerController::faster );
#ifndef __APPLE__ /* No icons in menus in Mac */
        action->setIcon( QStringLiteral(":/menu/ic_fluent_fast_forward.svg") );
#endif
    }

    action = rateMenu->addAction( QStringLiteral( ":/menu/ic_fluent_fast_forward.svg" ), qtr( "Faster (fine)" ), THEMIM,
                              &PlayerController::littlefaster );

    action = rateMenu->addAction( qtr( "N&ormal Speed" ), THEMIM,
                              &PlayerController::normalRate );

    action = rateMenu->addAction( QStringLiteral( ":/menu/ic_fluent_rewind.svg" ), qtr( "Slower (fine)" ), THEMIM,
                              &PlayerController::littleslower );

    if( b_normal )
    {
        action = rateMenu->addAction( qtr( "Slo&wer" ), THEMIM,
                                  &PlayerController::slower );
#ifndef __APPLE__ /* No icons in menus in Mac */
        action->setIcon( QStringLiteral(":/menu/ic_fluent_rewind.svg") );
#endif
    }

    platformAgnosticMenu->addMenu( rateMenu );

    platformAgnosticMenu->addSeparator();

    if( !b_normal ) return;

    action = rateMenu->addAction( qtr( "&Jump Forward" ), THEMIM,
             &PlayerController::jumpFwd );
#ifndef __APPLE__ /* No icons in menus in Mac */
    action->setIcon( QStringLiteral(":/menu/ic_fluent_skip_forward_10.svg") );
#endif

    action = rateMenu->addAction( qtr( "Jump Bac&kward" ), THEMIM,
             &PlayerController::jumpBwd );
#ifndef __APPLE__ /* No icons in menus in Mac */
    action->setIcon( QStringLiteral(":/menu/ic_fluent_skip_back_10.svg") );
#endif

    action = platformAgnosticMenu->addAction( qfut( I_MENU_GOTOTIME ), THEDP, &DialogsProvider::gotoTimeDialog, qtr( "Ctrl+T" ) );

    platformAgnosticMenu->addSeparator();
}

template<class Menu, class Action>
void VLCMenuBar::PopupMenuStaticEntries( Menu *menu )
{
    const auto openmenu = PlatformAgnosticMenu::createMenu( qtr( "Open Media" ), menu );
    assert(openmenu);
    addDPStaticEntry( openmenu, qfut( I_OP_OPF ),
        QStringLiteral(":/menu/file.svg"), &DialogsProvider::openFileDialog);
    addDPStaticEntry( openmenu, qfut( I_OP_OPDIR ),
        QStringLiteral(":/menu/folder.svg"), &DialogsProvider::PLOpenDir);
    addDPStaticEntry( openmenu, qtr( "Open &Disc..." ),
        QStringLiteral(":/menu/disc.svg"), &DialogsProvider::openDiscDialog);
    addDPStaticEntry( openmenu, qtr( "Open &Network..." ),
        QStringLiteral(":/menu/network.svg"), &DialogsProvider::openNetDialog);
    addDPStaticEntry( openmenu, qtr( "Open &Capture Device..." ),
        QStringLiteral(":/menu/capture-card.svg"), &DialogsProvider::openCaptureDialog);
    PlatformAgnosticMenu::fromMenu(menu)->addMenu( openmenu );

    menu->addSeparator();

    addDPStaticEntry( menu, qtr( "Quit" ), QStringLiteral(":/menu/exit.svg"),
                      &DialogsProvider::quit, "Ctrl+Q", PlatformAgnosticAction::QuitRole );
}

/* Video Tracks and Subtitles tracks */
PlatformAgnosticMenu* VLCMenuBar::VideoPopupMenu( qt_intf_t *p_intf, bool show )
{
    assert(p_intf);
    assert(p_intf->p_mi);
    const auto menu = PlatformAgnosticMenu::createMenu( p_intf->p_mi->quickWindow() );

    VideoMenu(p_intf, menu);
    if( show )
        menu->popup( QCursor::pos() );
    return menu;
}

/* Audio Tracks */
PlatformAgnosticMenu* VLCMenuBar::AudioPopupMenu( qt_intf_t *p_intf, bool show )
{
    assert(p_intf);
    assert(p_intf->p_mi);
    const auto menu = PlatformAgnosticMenu::createMenu( p_intf->p_mi->quickWindow() );

    AudioMenu(p_intf, menu);
    if( show )
        menu->popup( QCursor::pos() );
    return menu;
}

/* Navigation stuff, and general menus ( open ), used only for skins */
PlatformAgnosticMenu* VLCMenuBar::MiscPopupMenu( qt_intf_t *p_intf, bool show )
{
    assert(p_intf);
    assert(p_intf->p_mi);
    const auto menu = PlatformAgnosticMenu::createMenu( p_intf->p_mi->quickWindow() );

    menu->addSeparator();
    PopupMenuPlaylistEntries( menu, p_intf );

    menu->addSeparator();
    PopupMenuControlEntries( menu, p_intf );

    menu->addSeparator();
    PopupMenuStaticEntries( menu );

    if( show )
        menu->popup( QCursor::pos() );
    return menu;
}

/* Main Menu that sticks everything together  */
PlatformAgnosticMenu* VLCMenuBar::PopupMenu( qt_intf_t *p_intf, bool show )
{
    assert(p_intf);
    assert(p_intf->p_mi);
    const auto menu = PlatformAgnosticMenu::createMenu( p_intf->p_mi->quickWindow() );

    input_item_t* p_input = THEMIM->getInput();
    PlatformAgnosticAction *action;
    bool b_isFullscreen = false;

    PopupMenuPlaylistEntries( menu, p_intf );
    menu->addSeparator();

    if( p_input )
    {
        PlatformAgnosticMenu *submenu;
        SharedVOutThread p_vout = THEMIM->getVout();

        /* Add a fullscreen switch button, since it is the most used function */
        if( p_vout )
        {
            b_isFullscreen = THEMIM->isFullscreen();
            if (b_isFullscreen)
                menu->addAction((*new BooleanPropertyAction(qtr( "Leave Fullscreen" ), THEMIM, "fullscreen", menu))() );
            else
                menu->addAction((*new BooleanPropertyAction(qtr( "&Fullscreen" ), THEMIM, "fullscreen", menu))() );

            menu->addSeparator();
        }

        /* Input menu */

        /* Audio menu */
        submenu = PlatformAgnosticMenu::createMenu( menu );
        AudioMenu( p_intf, submenu );
        menu->addMenu( submenu );
        submenu->setTitle( qtr( "&Audio" ) );
        if( submenu->isEmpty() )
            submenu->setEnabled( false );

        /* Video menu */
        submenu = PlatformAgnosticMenu::createMenu( menu );
        VideoMenu( p_intf, submenu );
        menu->addMenu( submenu );
        submenu->setTitle( qtr( "&Video" ) );
        if( submenu->isEmpty() )
            submenu->setEnabled( false );

        /* Subtitles menu */
        submenu = PlatformAgnosticMenu::createMenu( menu );
        SubtitleMenu( p_intf, submenu, true );
        menu->addMenu( submenu );
        submenu->setTitle( qtr( "Subti&tle") );

        /* Playback menu for chapters */
        submenu = PlatformAgnosticMenu::createMenu( menu );
        NavigMenu( p_intf, submenu );
        menu->addMenu( submenu );
        submenu->setTitle( qtr( "&Playback" ) );
        if( submenu->isEmpty() )
            submenu->setEnabled( false );
    }

    menu->addSeparator();

    /* Add some special entries for windowed mode: Interface Menu */
    if( !b_isFullscreen )
    {
        if( p_intf->b_isDialogProvider )
        {
            // same as Tool menu but with extra entries
            PlatformAgnosticMenu* submenu = PlatformAgnosticMenu::createMenu( menu );
            submenu->setTitle(qtr( "Interface" ));
            ToolsMenu( p_intf, submenu );
            submenu->addSeparator();

            vlc_object_t* p_object = vlc_object_parent(p_intf->intf);

            /* Open skin dialog box */
            if (var_Type(p_object, "intf-skins-interactive") & VLC_VAR_ISCOMMAND)
            {
                PlatformAgnosticAction* openSkinAction = PlatformAgnosticAction::createAction(qtr("Open skin..."), menu);
                openSkinAction->setShortcut( QKeySequence( "Ctrl+Shift+S" ));
                connect(openSkinAction, &PlatformAgnosticAction::triggered, [=]() {
                    var_TriggerCallback(p_object, "intf-skins-interactive");
                });
                submenu->addAction(openSkinAction);
            }

            VLCVarChoiceModel* skinmodel = new VLCVarChoiceModel(p_object, "intf-skins", submenu);
            CheckableListMenu* skinsubmenu = new CheckableListMenu(qtr("Interface"), skinmodel, CheckableListMenu::GROUPED_OPTIONAL, submenu);
            submenu->addMenu((*skinsubmenu)());

            submenu->addSeparator();

            /* list of extensions */
            ExtensionsMenu( p_intf, submenu );

            menu->addMenu( submenu );
        }
        else
        {
            PlatformAgnosticMenu* toolsMenu = PlatformAgnosticMenu::createMenu( menu );
            toolsMenu->setTitle("Tools");
            ToolsMenu( p_intf, toolsMenu );
            menu->addMenu( toolsMenu );

            PlatformAgnosticMenu* viewMenu = PlatformAgnosticMenu::createMenu( menu );
            viewMenu->setTitle("View");
            ViewMenu( p_intf, viewMenu );
            menu->addMenu( viewMenu );
        }
    }

    /* Static entries for ending, like open */
    if( p_intf->b_isDialogProvider )
    {

        PlatformAgnosticMenu* openmenu = PlatformAgnosticMenu::createMenu( menu );
        openmenu->setTitle("Open");
        FileMenu( p_intf, openmenu );
        menu->addMenu( openmenu );

        openmenu->addSeparator();

        PlatformAgnosticMenu* helpmenu = PlatformAgnosticMenu::createMenu( menu );
        helpmenu->setTitle("Help");
        HelpMenu( helpmenu );
        menu->addMenu( helpmenu );

        addDPStaticEntry( menu, qtr( "Quit" ), QStringLiteral(":/menu/exit.svg"),
                          &DialogsProvider::quit, "Ctrl+Q", PlatformAgnosticAction::QuitRole );
    }
    else
        PopupMenuStaticEntries( menu );

    if( show )
        menu->popup( QCursor::pos() );
    return menu;
}

/************************************************************************
 * Systray Menu                                                         *
 ************************************************************************/

void VLCMenuBar::updateSystrayMenu( MainCtx *mi,
                                  qt_intf_t *p_intf,
                                  bool b_force_visible )
{
    /* Get the systray menu and clean it */
    QMenu *sysMenu = mi->getSysTrayMenu();
    // explictly delete submenus, see QTBUG-11070
    for (QAction *action : sysMenu->actions()) {
        if (action->menu()) {
            delete action->menu();
        }
    }
    sysMenu->clear();

#ifndef Q_OS_MAC
    /* Hide / Show VLC and cone */
    if( mi->isInterfaceVisible() || b_force_visible )
    {
        sysMenu->addAction( QIcon( QStringLiteral(":/logo/vlc16.png") ),
                            qtr( "&Hide VLC media player in taskbar" ), mi,
                            &MainCtx::hideUpdateSystrayMenu);
    }
    else
    {
        sysMenu->addAction( QIcon( QStringLiteral(":/logo/vlc16.png") ),
                            qtr( "Sho&w VLC media player" ), mi,
                            &MainCtx::showUpdateSystrayMenu);
    }
    sysMenu->addSeparator();
#endif

    PopupMenuPlaylistEntries( sysMenu, p_intf );
    PopupMenuControlEntries( sysMenu, p_intf, false );

    VolumeEntries( p_intf, sysMenu );
    sysMenu->addSeparator();
    addDPStaticEntry( sysMenu, qtr( "&Open Media" ),
        QStringLiteral(":/menu/file.svg"), &DialogsProvider::openFileDialog);
    addDPStaticEntry( sysMenu, qtr( "&Quit" ) ,
        QStringLiteral(":/menu/exit.svg"), &DialogsProvider::quit);

    /* Set the menu */
    mi->getSysTray()->setContextMenu( sysMenu );
}



/*****************************************************************************
 * Private methods.
 *****************************************************************************/

template<class ActionGroup, class Menu, class Action>
void VLCMenuBar::updateAudioDevice( qt_intf_t * p_intf, Menu *current )
{
    char **ids, **names;
    char *selected;

    if( !current )
        return;

    current->clear();
    SharedAOut aout = THEMIM->getAout();
    if (!aout)
        return;

    int i_result = aout_DevicesList( aout.get(), &ids, &names);
    if( i_result < 0 )
        return;

    selected = aout_DeviceGet( aout.get() );

    const auto actionGroup = PlatformAgnosticActionGroup::createActionGroup(current);
    PlatformAgnosticAction *action;

    for( int i = 0; i < i_result; i++ )
    {
        action = PlatformAgnosticAction::createAction( qfue( names[i] ), actionGroup );
        action->setData( qfu(ids[i]) );
        action->setCheckable( true );
        if( (selected && !strcmp( ids[i], selected ) ) ||
            (selected == NULL && ids[i] && ids[i][0] == '\0' ) )
            action->setChecked( true );
        actionGroup->addAction( action );
        current->addAction( action );
        connect(action, &Action::triggered, THEMIM->menusAudioMapper, QOverload<>::of(&QSignalMapper::map));
        THEMIM->menusAudioMapper->setMapping(action, ids[i]);
        free( ids[i] );
        free( names[i] );
    }
    free( ids );
    free( names );
    free( selected );
}
