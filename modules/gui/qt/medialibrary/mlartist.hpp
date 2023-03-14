/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MLARTIST_HPP
#define MLARTIST_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "vlc_common.h"

#include <QObject>
#include <QString>
#include <memory>
#include <vlc_media_library.h>

#include "mlalbum.hpp"
#include "mlhelper.hpp"
#include "mlqmltypes.hpp"

class MLArtist : public QObject, public MLItem
{
    Q_OBJECT

public:
    MLArtist(const vlc_ml_artist_t *_data, QObject *_parent = nullptr);

    QString getName() const;
    QString getShortBio() const;
    QString getCover() const;
    unsigned int getNbAlbums() const;
    unsigned int getNbTracks() const;

    Q_INVOKABLE QString getPresName() const;
    Q_INVOKABLE QString getPresImage() const;
    Q_INVOKABLE QString getPresInfo() const;

private:
    QString m_name;
    QString m_shortBio;
    QString m_cover;
    unsigned int m_nbAlbums;
    unsigned int m_nbTracks;
};

#endif
