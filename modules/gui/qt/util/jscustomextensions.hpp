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
#ifndef JSCUSTOMEXTENSIONS_HPP
#define JSCUSTOMEXTENSIONS_HPP

#include <QObject>
#include <QJSEngine>
#include <QPointer>

#ifdef Q_MOC_RUN
# define EXPORT(Name) Name
#else
# define EXPORT(Name)
#endif

class JSCustomExtensions final : public QObject
{
    Q_OBJECT
public:
    explicit JSCustomExtensions(QJSEngine &jsEngine, QObject *parent);

    ~JSCustomExtensions();


private:
    const QPointer<QJSEngine> engine;

};

#undef EXPORT

#endif // JSCUSTOMEXTENSIONS_HPP
