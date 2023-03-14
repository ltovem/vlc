/*****************************************************************************
 * registry.hpp: Windows Registry Manipulation
 ****************************************************************************
 * Copyright (C) 2008 the VideoLAN team
 *
 * Authors: Andre Weber <WeberAndre # gmx - de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef QVLC_REGISTRY_H
#define QVLC_REGISTRY_H

#include <windows.h>

class QVLCRegistry
{
private:
    const HKEY m_RootKey;
public:
    QVLCRegistry(HKEY rootKey);
    ~QVLCRegistry(void) {}

    void WriteRegistry( const char *path, const char *valueName, int value);
    void WriteRegistry( const char *path, const char *valueName, const char *value);
    void WriteRegistry( const char *path, const char *valueName, double value);

    int ReadRegistry( const char *path, const char *valueName, int default_value);
    char * ReadRegistry( const char *path, const char *valueName, const char *default_value);
    double ReadRegistry( const char *path, const char *valueName, double default_value);

    bool RegistryKeyExists( const char *path);
    bool RegistryValueExists( const char *path, const char *valueName);
    int DeleteValue( const char *path, const char *valueName );
    long DeleteKey( const char *path, const char *keyName );
};

#endif
