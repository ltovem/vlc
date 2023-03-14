/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MLHELPER_HPP
#define MLHELPER_HPP

#include <memory>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "vlc_media_library.h"
#include <QString>

// Forward declarations
class MLBaseModel;
class MLItemId;
class CoverGenerator;
class QUrl;

template<typename T>
class MLDeleter
{
public:
    void operator() (T* obj) {
        vlc_ml_release(obj);
    }
};

template<typename T>
using ml_unique_ptr = std::unique_ptr<T, MLDeleter<T> >;

template<typename T>
class MLListRange
{
public:
    MLListRange( T* begin, T* end )
        : m_begin(begin)
        , m_end(end)
    {
    }

    T* begin() const
    {
        return m_begin;
    }

    T* end() const
    {
        return m_end;
    }

private:
    T* m_begin;
    T* m_end;
};

template<typename T, typename L>
MLListRange<T> ml_range_iterate(L& list)
{
    return MLListRange<T>{ list->p_items, list->p_items + list->i_nb_items };
}

QString toValidLocalFile(const char *mrl);

template <typename T, typename O>
void thumbnailCopy(const MLListRange<T> &list, O dst, const int max)
{
    int count = 0;
    for (const auto &item : list)
    {
        if (item.thumbnails[VLC_ML_THUMBNAIL_SMALL].i_status != VLC_ML_THUMBNAIL_STATUS_AVAILABLE)
            continue;

        const auto path = toValidLocalFile(item.thumbnails[VLC_ML_THUMBNAIL_SMALL].psz_mrl);
        if (path.isEmpty())
            continue;

        *dst++ = path;
        ++count;
        if (count == max)
            return;
    }
}

QString urlToDisplayString(const QUrl &url);

#endif // MLHELPER_HPP
