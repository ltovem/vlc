/*****************************************************************************
 * validators.hpp : Custom Input validators
 ****************************************************************************
 * Copyright (C) 2006-2013 the VideoLAN team
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VALIDATORS_HPP
#define VALIDATORS_HPP

#include "qt.hpp"

#include <QValidator>

class UrlValidator : public QValidator
{
   Q_OBJECT
public:
   UrlValidator( QObject *parent ) : QValidator( parent ) { }
   QValidator::State validate( QString&, int& ) const override;
   void fixup ( QString & input ) const override;
};

#endif // VALIDATORS_HPP
