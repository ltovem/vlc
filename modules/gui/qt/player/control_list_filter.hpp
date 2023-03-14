/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef CONTROLLISTFILTER_HPP
#define CONTROLLISTFILTER_HPP

// Qt includes
#include <QSortFilterProxyModel>

// Forward declarations
class PlayerController;
class MainCtx;

class ControlListFilter : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(PlayerController * player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(MainCtx* ctx READ ctx WRITE setCtx NOTIFY ctxChanged)

public:
    explicit ControlListFilter(QObject * parent = nullptr);

public: // QAbstractProxyModel reimplementation
    void setSourceModel(QAbstractItemModel * sourceModel) override;

protected: // QSortFilterProxyModel reimplementation
    bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const override;

signals:
    void playerChanged();
    void ctxChanged();

public: // Properties
    PlayerController * player();
    void setPlayer(PlayerController * player);

    MainCtx* ctx() const;
    void setCtx(MainCtx* ctx);

private: // Variables
    PlayerController * m_player = nullptr;
    MainCtx* m_ctx = nullptr;
};

#endif // CONTROLLISTFILTER_HPP
