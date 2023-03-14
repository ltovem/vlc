/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#include "mlhelper.hpp"

// MediaLibrary includes
#include "mlbasemodel.hpp"

#include <QDir>

QString toValidLocalFile(const char *mrl)
{
    QUrl url(mrl);
    return url.isLocalFile() ? url.toLocalFile() : QString {};
}

QString urlToDisplayString(const QUrl &url)
{
    const QString displayString = url.toDisplayString(QUrl::RemovePassword | QUrl::PreferLocalFile | QUrl::NormalizePathSegments);
    if (url.isLocalFile())
        return QDir::toNativeSeparators(displayString);

    return displayString;
}
