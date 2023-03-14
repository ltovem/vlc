/*****************************************************************************
 * Access to vlc_gettext from QML
 ****************************************************************************
 * Copyright (C) 2019 the VideoLAN team
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "i18n.hpp"
#include <vlc_common.h>

#ifdef qtr
#undef qtr
#endif

I18n::I18n(QObject *parent)
    : QObject(parent)
{
}

QString I18n::qtr(const QString msgid) const
{
    //we need msgIdUtf8 to stay valid for the whole scope,
    //as vlc_gettext may return the incoming pointer
    QByteArray msgIdUtf8 = msgid.toUtf8();
    const char * msgstr_c = vlc_gettext(msgIdUtf8.constData());
    return QString::fromUtf8( msgstr_c );
}
