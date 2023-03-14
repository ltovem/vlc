/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MLGENRE_HPP
#define MLGENRE_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// MediaLibrary includes
#include "mlqmltypes.hpp"

class MLGenre : public MLItem
{
public:
    MLGenre(const vlc_ml_genre_t * _data);

    QString getName() const;

    unsigned int getNbTracks() const;

private:
    QString m_name;

    unsigned int m_nbTracks;
};

#endif
