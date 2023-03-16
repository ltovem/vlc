// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *****************************************************************************/

#ifndef MLFOLDER_HPP
#define MLFOLDER_HPP

// MediaLibrary includes
#include "mlqmltypes.hpp"
#include "util/vlctick.hpp"

class MLFolder : public MLItem
{
public:
    MLFolder(const vlc_ml_folder_t * data);

public: // Interface
    bool isPresent() const;
    bool isBanned() const;

    QString getTitle() const;

    QString getMRL() const;

    VLCTick getDuration() const;

    unsigned int getCount() const;

    unsigned int getAudioCount() const;

    unsigned int getVideoCount() const;

private:
    bool m_present;
    bool m_banned;

    QString m_title;

    QString m_mrl;

    int64_t m_duration;

    unsigned int m_count;
    unsigned int m_audioCount;
    unsigned int m_videoCount;
};

#endif
