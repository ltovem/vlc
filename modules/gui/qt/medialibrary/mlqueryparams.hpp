/*****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MLQUERYPARAMS_HPP
#define MLQUERYPARAMS_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <QByteArray>
#include "vlc_media_library.h"

/**
 * C++ owned version of vlc_ml_query_params_t, so that it can be moved or
 * copied.
 */
class MLQueryParams
{
public:
    MLQueryParams(QByteArray searchPatternUtf8, vlc_ml_sorting_criteria_t sort,
                  bool desc, size_t index, size_t count)
        : searchPatternUtf8(std::move(searchPatternUtf8))
        , nbResults(count)
        , offset(index)
        , sort(sort)
        , desc(desc)
    {
    }

    MLQueryParams(QByteArray patternUtf8, vlc_ml_sorting_criteria_t sort,
                  bool desc)
        : MLQueryParams(std::move(patternUtf8), sort, desc, 0, 0)
    {
    }

    /**
     * Expose the MLQueryParams content to a vlc_ml_query_params_t.
     *
     * The returned value is valid as long as the MLQueryParams instance is
     * alive.
     */
    vlc_ml_query_params_t toCQueryParams() const;

private:
    QByteArray searchPatternUtf8;
    uint32_t nbResults;
    uint32_t offset;
    vlc_ml_sorting_criteria_t sort;
    bool desc;
};

#endif
