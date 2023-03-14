/*****************************************************************************
 * simple_preferences.hpp : Simple prefs
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_QT_SIMPLE_PREFERENCES_HPP_
#define VLC_QT_SIMPLE_PREFERENCES_HPP_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>

/* Auto-generated from .ui files */
#include "ui_sprefs_input.h"
#include "ui_sprefs_audio.h"
#include "ui_sprefs_video.h"
#include "ui_sprefs_subtitles.h"
#include "ui_sprefs_interface.h"
#include "ui_sprefs_medialibrary.h"

#include <vlc_media_library.h>

class MLFoldersEditor;

#ifdef _WIN32
# include "util/registry.hpp"
#endif

#include <QWidget>

enum {
    SPrefsInterface = 0,
    SPrefsAudio,
    SPrefsVideo,
    SPrefsSubtitles,
    SPrefsInputAndCodecs,
    SPrefsHotkeys,
    SPrefsMediaLibrary,
    SPrefsMax
};
#define SPrefsDefaultCat SPrefsInterface

enum {
    CachingCustom = 0,
    CachingLowest = 100,
    CachingLow    = 200,
    CachingNormal = 300,
    CachingHigh   = 500,
    CachingHigher = 1000
};

class ConfigControl;
class QString;

#ifdef _WIN32
class QTreeWidgetItem;
#endif

class SPrefsCatList : public QWidget
{
    Q_OBJECT
public:
    SPrefsCatList( qt_intf_t *, QWidget * );
    virtual ~SPrefsCatList() {};
private:
    qt_intf_t *p_intf;
signals:
    void currentItemChanged( int );
public slots:
    void switchPanel( int );
};

class SPrefsPanel : public QWidget
{
    Q_OBJECT
public:
    SPrefsPanel( qt_intf_t *, QWidget *, int );
    virtual ~SPrefsPanel();
    void apply();
#ifdef _WIN32
    void cleanLang();
#endif

private:
    qt_intf_t *p_intf;
    QList<ConfigControl *> controls;

    int number;

    QHash<QString, QWidget*> optionWidgets;
    QStringList qs_filter;
    QButtonGroup *radioGroup;

    char *lang;
    MLFoldersEditor *mlFoldersEditor {};
    MLFoldersEditor *mlBannedFoldersEditor {};

#ifdef _WIN32
    QList<QTreeWidgetItem *> listAsso;
    bool addType( const char * psz_ext, QTreeWidgetItem*, QTreeWidgetItem*, QVLCRegistry* );
    void saveLang();
#endif

    // used to revert properties on cancel which are immediately set
    bool m_isApplied = false;
    std::vector<std::unique_ptr<class PropertyResetter>> m_resetters;

/* Display only the options for the selected audio output */
private slots:
    void lastfm_Changed( int );
    void updateAudioOptions( int );
    void updateAudioVolume( int );
    void langChanged( int );
#ifdef _WIN32
    void assoDialog();
    void updateCheckBoxes( QTreeWidgetItem*, int );
    void saveAsso();
#endif
    void MLaddNewFolder( );
    void MLBanFolder( );

    void configML();
    void changeStyle( );

    void clean();
};

#endif
