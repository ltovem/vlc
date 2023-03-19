// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

#pragma once

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "vlc_common.h"

#include <QObject>
#include <QString>
#include <QList>
#include <memory>
#include "vlc_media_library.h"
#include "mlhelper.hpp"
#include "mlqmltypes.hpp"
#include "util/vlctick.hpp"

class MLAlbum : public QObject, public MLItem
{
    Q_OBJECT

public:
    MLAlbum(const vlc_ml_album_t *_data, QObject *_parent = nullptr);

    QString getTitle() const;
    unsigned int getReleaseYear() const;
    QString getShortSummary() const;
    QString getCover() const;
    QString getArtist() const;
    unsigned int getNbTracks() const;
    VLCTick getDuration() const;

    Q_INVOKABLE QString getPresName() const;
    Q_INVOKABLE QString getPresImage() const;
    Q_INVOKABLE QString getPresInfo() const;

private:
    QString m_title;
    unsigned int m_releaseYear;
    QString m_shortSummary;
    QString m_cover;
    QString m_mainArtist;
    QList<QString> m_otherArtists;
    unsigned int m_nbTracks;
    int64_t m_duration;
};
