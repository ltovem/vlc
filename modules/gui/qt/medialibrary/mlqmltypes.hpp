/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef MLQMLTYPES_HPP
#define MLQMLTYPES_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <QObject>
#include <vlc_common.h>
#include <vlc_media_library.h>

class MLItemId
{
    Q_GADGET
public:
    MLItemId() : id(0), type( VLC_ML_PARENT_UNKNOWN ) {}
    MLItemId( int64_t i, vlc_ml_parent_type t ) : id( i ), type( t ) {}
    bool operator==( const MLItemId& other )
    {
        return id == other.id && type == other.type;
    }
    bool operator!=( const MLItemId& other )
    {
        return !(*this == other);
    }
    int64_t id;
    vlc_ml_parent_type type;

    Q_INVOKABLE inline QString toString() const {

#define ML_PARENT_TYPE_CASE(type) case type: return QString("%1 - %2").arg(#type).arg(id)
        switch (type) {
            ML_PARENT_TYPE_CASE(VLC_ML_PARENT_ALBUM);
            ML_PARENT_TYPE_CASE(VLC_ML_PARENT_ARTIST);
            ML_PARENT_TYPE_CASE(VLC_ML_PARENT_SHOW);
            ML_PARENT_TYPE_CASE(VLC_ML_PARENT_GENRE);
            ML_PARENT_TYPE_CASE(VLC_ML_PARENT_GROUP);
            ML_PARENT_TYPE_CASE(VLC_ML_PARENT_FOLDER);
            ML_PARENT_TYPE_CASE(VLC_ML_PARENT_PLAYLIST);
        default:
            return QString("UNKNOWN - %2").arg(id);
        }
#undef ML_PARENT_TYPE_CASE
    }
};

Q_DECLARE_METATYPE(MLItemId)

class MLItem
{
public:
    MLItem(MLItemId id) : m_id(id) {}
    virtual ~MLItem() = default;

    MLItemId getId() const { return m_id; };

private:
    MLItemId m_id;
};

#endif // MLQMLTYPES_HPP
