/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#pragma once

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "vlc_common.h"

#include <QObject>
#include <QString>
#include <memory>

#include <vlc_media_library.h>
#include "mlhelper.hpp"
#include "mlqmltypes.hpp"
#include "util/vlctick.hpp"

class MLAlbumTrack : public QObject, public MLItem
{
    Q_OBJECT

public:
    MLAlbumTrack(vlc_medialibrary_t *_ml, const vlc_ml_media_t *_data, QObject *_parent = nullptr);

    QString getTitle() const;
    QString getAlbumTitle() const;
    QString getArtist() const;
    QString getCover() const;
    unsigned int getTrackNumber() const;
    unsigned int getDiscNumber() const;
    VLCTick getDuration() const;
    QString getMRL() const;

private:
    QString m_title;
    QString m_albumTitle;
    QString m_artist;
    QString m_cover;
    unsigned int m_trackNumber;
    unsigned int m_discNumber;
    int64_t m_duration;
    QString m_mrl;

   ml_unique_ptr<vlc_ml_media_t> m_data;
};
