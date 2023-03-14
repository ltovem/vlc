/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef LISTCACHELOADER_HPP
#define LISTCACHELOADER_HPP

struct vlc_medialibrary_t;

template <typename T>
struct ListCacheLoader
{
    virtual ~ListCacheLoader() = default;
    virtual size_t count(vlc_medialibrary_t* ml) const = 0;
    virtual std::vector<T> load(vlc_medialibrary_t* ml, size_t index, size_t count) const = 0;
};

#endif
