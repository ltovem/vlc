/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#include "jscustomextensions.hpp"

#include <QMetaObject>
#include <QMetaMethod>

#define restrict
#include "vlc_common.h"

JSCustomExtensions::JSCustomExtensions(QJSEngine& jsEngine, QObject *parent)
    : QObject{parent}
    , engine(&jsEngine)
{
    const auto& thisAsQObject = jsEngine.newQObject(this);

    for (int i = 0; i < staticMetaObject.methodCount(); i++)
    {
        const QMetaMethod& metaMethod = staticMetaObject.method(i);
        const auto& methodTag = QLatin1String(metaMethod.tag());
        const auto& methodName = QLatin1String(metaMethod.name());

        if (methodTag.isEmpty())
            continue;

        QJSValue&& globalObject = jsEngine.globalObject();
        QJSValue group;

        if (!globalObject.hasProperty(methodTag))
        {
            group = jsEngine.newObject();
        }
        else
        {
            group = globalObject.property(methodTag);
        }

        group.setProperty(methodName, thisAsQObject.property(methodName));

        globalObject.setProperty(methodTag, group);
    }
}

JSCustomExtensions::~JSCustomExtensions()
{
    // This class must outlive the javascipt engine.
    assert(engine.isNull());
}

QString JSCustomExtensions::qtr(const QString& msgid)
{
    return QString::fromUtf8(vlc_gettext(msgid.toUtf8().constData()));
}
