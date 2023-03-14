/*****************************************************************************
 * Access to vlc_gettext from QML
 ****************************************************************************
 * Copyright (C) 2019 the VideoLAN team
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef I18N_HPP
#define I18N_HPP

#include <QString>
#include <QObject>

class I18n : public QObject
{
    Q_OBJECT
public:
    I18n(QObject* parent = nullptr);
public:
#ifdef qtr
#undef qtr
    Q_INVOKABLE QString qtr(QString msgid) const;
#define qtr(i) QString::fromUtf8( vlc_gettext(i) )
#else
    Q_INVOKABLE QString qtr(const QString msgid) const;
#endif

};

#endif // I18N_HPP
