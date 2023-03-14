/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_FIRSTRUNWIZARD_H
#define VLC_FIRSTRUNWIZARD_H

#include "ui_firstrunwizard.h"
#include "util/singleton.hpp"

#include <QWizard>

class MLFoldersEditor;
class MLFoldersModel;

class FirstRunWizard : public QWizard, public Singleton<FirstRunWizard>
{
    Q_OBJECT
private:
    explicit FirstRunWizard ( qt_intf_t*, QWidget* parent = nullptr );
    enum { WELCOME_PAGE, FOLDER_PAGE, COLOR_SCHEME_PAGE, LAYOUT_PAGE };
    enum { MODERN, CLASSIC };

    void addDefaults();

    int nextId() const;
    void initializePage( int id );
    void reject();

private:
    Ui::firstrun ui;

    MLFoldersEditor *mlFoldersEditor = nullptr;
    MLFoldersModel *mlFoldersModel = nullptr;

    qt_intf_t* p_intf;
    bool mlDefaults = false;

    QButtonGroup* colorSchemeGroup = nullptr;
    QButtonGroup* colorSchemeImages = nullptr;
    QButtonGroup* layoutImages = nullptr;

private slots:
    void finish();
    void MLaddNewFolder();
    void updateColorLabel( QAbstractButton* );
    void updateLayoutLabel (QAbstractButton* );
    void imageColorSchemeClick ( QAbstractButton* );
    void imageLayoutClick( QAbstractButton* );

    friend class Singleton<FirstRunWizard>;
};

#endif // VLC_FIRSTRUNWIZARD_H
