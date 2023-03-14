/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef AUDIO_DEVICE_MODEL_HPP
#define AUDIO_DEVICE_MODEL_HPP

#include <QAbstractListModel>

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt.hpp"
#include <vlc_aout.h>

class AudioDeviceModel : public QAbstractListModel
{
    Q_OBJECT

public:
    AudioDeviceModel(vlc_player_t *player, QObject *parent = nullptr);

    ~AudioDeviceModel();

    virtual Qt::ItemFlags flags(const QModelIndex &) const  override;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void updateCurrent(QString current);

    QHash<int, QByteArray> roleNames() const override;

private:
    int m_inputs = 0;
    char **m_names = nullptr;
    char **m_ids = nullptr;
    QString m_current;
    vlc_player_aout_listener_id* m_player_aout_listener = nullptr;
    audio_output_t* m_aout = nullptr;
    vlc_player_t *m_player;
};

#endif // AUDIO_DEVICE_MODEL_HPP
