// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * playlists.hpp : playlists
 *****************************************************************************
 * Copyright (C) 2021 the VideoLAN team
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *****************************************************************************/

#ifndef QVLC_PLAYLISTS_H_
#define QVLC_PLAYLISTS_H_ 1

// VLC includes
#include <widgets/native/qvlcframe.hpp>
#include <util/singleton.hpp>

// Forward declarations
class MLPlaylistListModel;
class QTreeView;
class QLineEdit;
class QLabel;
class QPushButton;

class PlaylistsDialog : public QVLCFrame, public Singleton<PlaylistsDialog>
{
    Q_OBJECT

private: // Ctor / dtor
    PlaylistsDialog(qt_intf_t *);

    ~PlaylistsDialog() override;

public: // Interface
    Q_INVOKABLE void setMedias(const QVariantList & medias);

protected: // Events
    void hideEvent(QHideEvent * event) override;

    void keyPressEvent(QKeyEvent * event) override;

private slots:
    void onClicked      ();
    void onDoubleClicked();

    void onTextEdited();

    void onAccepted();

private:
    QVariantList m_ids;

    MLPlaylistListModel * m_model;

    QTreeView * m_playlists;
    QLineEdit * m_lineEdit;

    QLabel * m_label;

    QPushButton * m_button;

private:
    friend class Singleton<PlaylistsDialog>;
};

#endif
