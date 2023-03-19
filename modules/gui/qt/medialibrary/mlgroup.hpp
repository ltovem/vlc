// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *****************************************************************************/

#ifndef MLGROUP_HPP
#define MLGROUP_HPP

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// MediaLibrary includes
#include "mlqmltypes.hpp"
#include "util/vlctick.hpp"

class MLGroup : public MLItem
{
public:
    MLGroup(const vlc_ml_group_t * data);

public: // Interface
    QString getTitle() const;

    VLCTick getDuration() const;

    unsigned int getDate() const;

    unsigned int getCount() const;

private:
    QString m_title;

    int64_t m_duration;

    unsigned int m_date;

    unsigned int m_count;
};

#endif
