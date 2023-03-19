// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * roundimage.hpp: Custom widgets
 *****************************************************************************
 * Copyright (C) 2021 the VideoLAN team
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *****************************************************************************/

#ifndef VLC_QT_MLFOLDERSEDITOR_HPP
#define VLC_QT_MLFOLDERSEDITOR_HPP

#include "qt.hpp"

#include <QTableWidget>

#include <memory>

#include <medialibrary/mlfoldersmodel.hpp>

class MLFoldersEditor : public QTableWidget
{
    Q_OBJECT

public:
    MLFoldersEditor( QWidget *parent = nullptr );

    void setMLFoldersModel( MLFoldersBaseModel *foldersModel );
    void add( const QUrl &mrl );

    // call 'commit' to apply changes
    void commit();

private slots:
    void handleOpFailure( int operation, const QUrl &url );
    void resetFolders();

private:
    void newRow(const QUrl &mrl);
    void removeMrlEntry(const QUrl &mrl);

    MLFoldersBaseModel *m_foldersModel = nullptr;

    // new entries to add/remove on 'commit' call
    QVector<QUrl> m_newEntries;
    QVector<QUrl> m_removeEntries;
};

#endif

