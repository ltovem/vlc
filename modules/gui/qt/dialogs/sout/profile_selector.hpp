/*****************************************************************************
 * profile_selector.hpp : A small profile selector and editor
 ****************************************************************************
 * Copyright (C) 2009 the VideoLAN team
 *
 * Authors: Jean-Baptiste Kempf <jb@videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_QT_PROFILE_SELECTOR_HPP_
#define VLC_QT_PROFILE_SELECTOR_HPP_

#include "qt.hpp"

#include <QWidget>
#include <QSet>
#include <QHash>

#include "widgets/native/qvlcframe.hpp"
#include "util/soutchain.hpp"

/* Auto-generated from .ui files */
#include "ui_profiles.h"

class QComboBox;

class VLCProfileSelector : public QWidget
{
    Q_OBJECT

public:
    VLCProfileSelector( QWidget *_parent );
    ~VLCProfileSelector();
    QString getMux() { return mux; }
    SoutChain getTranscode() { return transcode; }
private:
    QComboBox *profileBox;
    void fillProfilesCombo();
    void editProfile( const QString&, const QString& );
    void saveProfiles();
    QString mux;
    SoutChain transcode;
private slots:
    void newProfile();
    void editProfile();
    void deleteProfile();
    void updateOptions( int i );
    void updateOptionsOldFormat( int i );
signals:
    void optionsChanged();
};

class VLCProfileEditor : public QVLCDialog
{
    Q_OBJECT

    Ui::Profiles ui;
public:
    VLCProfileEditor(const QString&, const QString&, QWindow * );

    QString name;
    QString muxValue;
    QString transcodeValue();
    QStringList qpcodecsList;
private:
    void registerCodecs();
    void registerFilters();
    void fillProfile( const QString& qs );
    void fillProfileOldFormat( const QString& qs );
    typedef QSet<QString> resultset;
    QHash<QString, resultset> caps;
    void loadCapabilities();
    void reset();
protected slots:
    void close() override;
private slots:
    void muxSelected();
    void codecSelected();
    void activatePanels();
    void fixBirateState();
    void fixQPState();
};

#endif
