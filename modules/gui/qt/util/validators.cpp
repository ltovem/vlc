/*****************************************************************************
 * validators.cpp : Custom Input validators
 ****************************************************************************
 * Copyright (C) 2006-2013 the VideoLAN team
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "validators.hpp"

#include <QUrl>

QValidator::State UrlValidator::validate( QString& str, int& ) const
{
    if( str.startsWith( ' ' ) )
        return QValidator::Invalid;

    if ( str.isEmpty() )
        return QValidator::Intermediate;

    QUrl url( str );
    if ( url.scheme().isEmpty() )
        return QValidator::Intermediate;

    return ( url.isValid() ) ? QValidator::Acceptable : QValidator::Intermediate;
}

void UrlValidator::fixup( QString & input ) const
{
    while( input.startsWith( ' ' ) )
        input.chop( 1 );
    QUrl fixed( input, QUrl::TolerantMode );
    input = fixed.toString();
}
