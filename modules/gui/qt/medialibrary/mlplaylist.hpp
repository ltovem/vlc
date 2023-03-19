// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef MLPLAYLIST_HPP
#define MLPLAYLIST_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// MediaLibrary includes
#include "mlqmltypes.hpp"
#include "util/vlctick.hpp"

class MLPlaylist : public MLItem
{
public:
    MLPlaylist(const vlc_ml_playlist_t * data);

public: // Interface
    QString getName() const;

    VLCTick getDuration() const;

    unsigned int getCount() const;

private:
    QString m_name;

    int64_t m_duration;

    unsigned int m_count;
};

#endif
