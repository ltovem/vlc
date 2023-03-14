/*****************************************************************************
 * qt_dirs.cpp : file path helpers
 ****************************************************************************
 * Copyright (C) 2010 the VideoLAN team
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "qt.hpp"
#include "qt_dirs.hpp"
#include <vlc_url.h>

QString toURI( const QString& s )
{
    if( s.contains( qfu("://") ) )
        return s;

    char *psz = vlc_path2uri( qtu(s), NULL );
    if( psz == NULL )
        return qfu("");

    QString uri = qfu( psz );
    free( psz );
    return uri;
}
