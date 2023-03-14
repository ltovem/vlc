/*****************************************************************************
 * extended.hpp : Extended controls - Undocked
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef QVLC_EXTENDED_DIALOG_H_
#define QVLC_EXTENDED_DIALOG_H_ 1

#include "widgets/native/qvlcframe.hpp"

#include "extended_panels.hpp"
#include "util/singleton.hpp"
#include "player/player_controller.hpp"

class QTabWidget;

class ExtendedDialog : public QVLCDialog, public Singleton<ExtendedDialog>
{
    Q_OBJECT
public:
    enum
    {
        AUDIO_TAB = 0,
        VIDEO_TAB,
        SYNCHRO_TAB,
        V4L2_TAB
    };
    void showTab( int i );
    int currentTab();
private:
    ExtendedDialog( qt_intf_t * );
    virtual ~ExtendedDialog();

    SyncControls *syncW;
    ExtVideo *videoEffect;
    Equalizer *equal;
    QTabWidget *mainTabW;
    QPushButton *m_applyButton;
    QHash<QString, QVariant> m_hashConfigs[2];
private slots:
    void changedItem(PlayerController::PlayingState );
    void currentTabChanged( int );
    void saveConfig();
    void putAudioConfig( const QString& name, const QVariant value );
    void putVideoConfig( const QString& name, const QVariant value );

    friend class    Singleton<ExtendedDialog>;
};

#endif
