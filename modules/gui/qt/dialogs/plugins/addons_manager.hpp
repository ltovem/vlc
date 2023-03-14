/*****************************************************************************
 * addons_manager.hpp: Addons manager for Qt
 ****************************************************************************
 * Copyright (C) 2013 VideoLAN and authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef ADDONS_MANAGER_HPP
#define ADDONS_MANAGER_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt.hpp"
#include "util/singleton.hpp"

#include <vlc_addons.h>

#include <QObject>
#include <QEvent>


class AddonManagerEvent : public QEvent
{
public:
    static const QEvent::Type AddedEvent;
    static const QEvent::Type ChangedEvent;
    static const QEvent::Type DiscoveryEndedEvent;

    AddonManagerEvent( QEvent::Type type, addon_entry_t *_p_entry )
        : QEvent( type ), p_entry( _p_entry )
    {
        addon_entry_Hold( p_entry );
    }
    virtual ~AddonManagerEvent()
    {
        addon_entry_Release( p_entry );
    }

    addon_entry_t *entry() const { return p_entry; }

private:
    addon_entry_t *p_entry;
};

class AddonsManager : public QObject, public Singleton<AddonsManager>
{
    Q_OBJECT
    friend class Singleton<AddonsManager>;

public:
    AddonsManager( qt_intf_t * );
    virtual ~AddonsManager();
    static void addonFoundCallback( addons_manager_t *, addon_entry_t * );
    static void addonsDiscoveryEndedCallback( addons_manager_t * );
    static void addonChangedCallback( addons_manager_t *, addon_entry_t * );
    void customEvent( QEvent * );
    void install( QByteArray id );
    void remove( QByteArray id );
    static QString getAddonType( int );

signals:
    void addonAdded( addon_entry_t * );
    void addonChanged( const addon_entry_t * );
    void discoveryEnded();

public slots:
    void findNewAddons();
    void findDesignatedAddon( QString uri );
    void findInstalled();

private:
    addons_manager_t* p_manager;
};

#endif // ADDONS_MANAGER_HPP
