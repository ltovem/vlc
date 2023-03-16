// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef VLC_QT_MEDIA_HPP_
#define VLC_QT_MEDIA_HPP_

#include <vlc_cxx_helpers.hpp>
#include <vlc_common.h>
#include <vlc_input_item.h>
#include <QString>
#include <QStringList>
#include "util/qt_dirs.hpp"

namespace vlc {
  namespace playlist {

using InputItemPtr = vlc_shared_data_ptr_type(input_item_t,
                                              input_item_Hold,
                                              input_item_Release);

class Media
{
public:
    Media(input_item_t *media = nullptr, const QStringList &options = {})
    {
        if (media)
        {
            /* the media must be unique in the playlist */
            ptr.reset(input_item_Copy(media), false);
            if (!ptr)
                throw std::bad_alloc();
            setMediaOptions(options);
        }
    }

    Media(QString uri, QString name, const QStringList &options = {})
    {
        auto uUri = uri.toUtf8();
        auto uName = name.toUtf8();
        const char *rawUri = uUri.isNull() ? nullptr : uUri.constData();
        const char *rawName = uName.isNull() ? nullptr : uName.constData();
        ptr.reset(input_item_New(rawUri, rawName), false);
        if (!ptr)
            throw std::bad_alloc();
        setMediaOptions(options);
    }

    void setMediaOptions(const QStringList &options = {})
    {
        if (options.count() > 0)
        {
            char **ppsz_options = NULL;
            int i_options = 0;

            ppsz_options = new char *[options.count()];
            auto optionDeleter = vlc::wrap_carray<char*>(ppsz_options, [&i_options](char *ptr[]) {
                for(int i = 0; i < i_options; i++)
                    free(ptr[i]);
                delete[] ptr;
            });

            for (int i = 0; i < options.count(); i++)
            {
                QString option = colon_unescape( options[i] );
                ppsz_options[i] = strdup(option.toUtf8().constData());
                if (!ppsz_options[i])
                    throw std::bad_alloc();
                i_options++;
            }
            input_item_AddOptions( ptr.get(), i_options, ppsz_options, VLC_INPUT_OPTION_TRUSTED );
        }
    }

    operator bool() const
    {
        return static_cast<bool>(ptr);
    }

    input_item_t *raw() const
    {
        return ptr.get();
    }

private:
    InputItemPtr ptr;
};

  } // namespace playlist
} // namespace vlc

#endif
