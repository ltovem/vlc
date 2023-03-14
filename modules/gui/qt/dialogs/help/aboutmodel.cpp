/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "aboutmodel.hpp"

#include "vlc_about.h"


AboutModel::AboutModel(QObject *parent) : QObject(parent)
{
}

QString AboutModel::getLicense() const
{
    return QString::fromUtf8(psz_license);
}

QString AboutModel::getAuthors() const
{
    return QString::fromUtf8(psz_authors);
}

QString AboutModel::getThanks() const
{
    return QString::fromUtf8(psz_thanks);
}

QString AboutModel::getVersion() const
{
    return QString::fromUtf8(VERSION_MESSAGE);

}
