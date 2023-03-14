/*****************************************************************************
 * extensions_manager.hpp: Extensions manager for Qt
 ****************************************************************************
 * Copyright (C) 2009-2010 VideoLAN and authors
 *
 * Authors: Jean-Philippe André < jpeg # videolan.org >
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EXTENSIONS_MANAGER_HPP
#define EXTENSIONS_MANAGER_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_extensions.h>

#include "qt.hpp"
#include <player/player_controller.hpp>
#include "util/singleton.hpp"

#include <QObject>

class QMenu;
class QSignalMapper;

class ExtensionsDialogProvider;

class ExtensionsManager : public QObject, public Singleton<ExtensionsManager>
{
    friend class Singleton<ExtensionsManager>;

    Q_OBJECT
public:
    inline bool isLoaded() { return p_extensions_manager != NULL; }
    inline bool cannotLoad() { return b_unloading || b_failed; }
    inline bool isUnloading() { return b_unloading; }
    void menu( QMenu *current );

    void openVLsub();

    /** Get the extensions_manager_t if it is loaded */
    extensions_manager_t* getManager()
    {
        return p_extensions_manager;
    }

private:
    ExtensionsManager( qt_intf_t *p_intf, QObject *parent = nullptr );
    virtual ~ExtensionsManager();

public slots:
    bool loadExtensions();
    void unloadExtensions();
    void reloadExtensions();

private slots:
    void triggerMenu( int id );
    void inputChanged( );
    void playingChanged(PlayerController::PlayingState );
    void metaChanged( input_item_t *p_input );

private:
    static ExtensionsManager* instance;
    qt_intf_t *p_intf;
    extensions_manager_t *p_extensions_manager;
    ExtensionsDialogProvider *p_edp;

    QSignalMapper *menuMapper;
    bool b_unloading;  ///< Work around threads + emit issues, see isUnloading
    bool b_failed; ///< Flag set to true if we could not load the module

signals:
    void extensionsUpdated();
};

#endif // EXTENSIONS_MANAGER_HPP
