// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef QMLINPUTITEM_HPP
#define QMLINPUTITEM_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

// VLC includes
#include <vlc_media_source.h>
#include <vlc_cxx_helpers.hpp>

// Qt includes
#include <QObject>

class QmlInputItem
{
    Q_GADGET

    using InputItemPtr = vlc_shared_data_ptr_type(input_item_t,
                                                  input_item_Hold,
                                                  input_item_Release);

public:
    QmlInputItem() : item(nullptr) {}

    QmlInputItem(input_item_t * item, bool hold) : item(item, hold) {}

public: // Operators
    QmlInputItem(const QmlInputItem &)  = default;
    QmlInputItem(QmlInputItem       &&) = default;

    QmlInputItem & operator=(const QmlInputItem &)  = default;
    QmlInputItem & operator=(QmlInputItem       &&) = default;

public: // Variables
    InputItemPtr item;
};

Q_DECLARE_METATYPE(QmlInputItem)

#endif // QMLINPUTITEM_HPP
