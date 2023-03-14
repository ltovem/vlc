/*****************************************************************************
 * variables.cpp : VLC variable class
 ****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "qt.hpp"
#include "variables.hpp"

void QVLCBool::setValue(bool value)
{
    setValueInternal(value);
}

void QVLCString::setValue(QString value)
{
    setValueInternal(value);
}

void QVLCFloat::setValue(float value)
{
    setValueInternal(value);
}

void QVLCInteger::setValue(int64_t value)
{
    setValueInternal(value);
}
