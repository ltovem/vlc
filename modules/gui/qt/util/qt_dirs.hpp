// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * qt_dirs.hpp : String Directory helpers
 *****************************************************************************
 * Copyright (C) 2006-2008 the VideoLAN team
 *
 * Authors:       Jean-Baptiste Kempf <jb@videolan.org>
 *****************************************************************************/

#ifndef VLC_QT_QT_DIRS_HPP_
#define VLC_QT_QT_DIRS_HPP_

#include <QString>
#include <QDir>

#define toNativeSeparators( s ) QDir::toNativeSeparators ( s )

static inline QString removeTrailingSlash( QString s )
{
    if( ( s.length() > 1 ) && ( s[s.length()-1] == QLatin1Char( '/' ) ) )
        s.remove( s.length() - 1, 1 );
    return s;
}

#define toNativeSepNoSlash( a ) toNativeSeparators( removeTrailingSlash( a ) )

static inline QString colon_escape( QString s )
{
    return s.replace( ":", "\\:" );
}
static inline QString colon_unescape( QString s )
{
    return s.replace( "\\:", ":" ).trimmed();
}

QString toURI( const QString& s );
#endif
