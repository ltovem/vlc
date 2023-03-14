/*****************************************************************************
 * registry.cpp: Windows Registry Manipulation
 ****************************************************************************
 * Copyright (C) 2008 the VideoLAN team
 *
 * Authors: Andre Weber <WeberAndre # gmx - de>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef _WIN32

#include <stdlib.h>
#include "registry.hpp"

QVLCRegistry::QVLCRegistry( HKEY rootKey )
    : m_RootKey( rootKey )
{}

bool QVLCRegistry::RegistryKeyExists( const char *path )
{
    HKEY keyHandle;
    if( RegOpenKeyExA( m_RootKey, path, 0, KEY_READ, &keyHandle ) == ERROR_SUCCESS )
    {
        RegCloseKey( keyHandle );
        return true;
    }
    return false;
}

bool QVLCRegistry::RegistryValueExists( const char *path, const char *valueName )
{
    HKEY keyHandle;
    bool temp = false;
    DWORD size1;
    DWORD valueType;

    if( RegOpenKeyExA( m_RootKey, path, 0, KEY_READ, &keyHandle ) == ERROR_SUCCESS )
    {
        if( RegQueryValueExA( keyHandle, valueName, NULL,
                             &valueType, NULL, &size1 ) == ERROR_SUCCESS )
        {
           temp = true;
        }
        RegCloseKey( keyHandle );
    }
    return temp;
}

void QVLCRegistry::WriteRegistry( const char *path, const char *valueName, int value )
{
    HKEY keyHandle;

    if( RegCreateKeyExA( m_RootKey, path, 0, NULL, REG_OPTION_NON_VOLATILE,
                         KEY_WRITE, NULL, &keyHandle, NULL )  == ERROR_SUCCESS )
    {
        RegSetValueExA( keyHandle, valueName, 0, REG_DWORD,
                (LPBYTE)&value, sizeof( int ) );
        RegCloseKey( keyHandle );
    }
}

void QVLCRegistry::WriteRegistry( const char *path, const char *valueName, const char *value )
{
    HKEY keyHandle;

    if( RegCreateKeyExA( m_RootKey, path, 0, NULL, REG_OPTION_NON_VOLATILE,
                         KEY_WRITE, NULL, &keyHandle, NULL )  == ERROR_SUCCESS )
    {
        RegSetValueExA( keyHandle, valueName, 0, REG_SZ, (LPBYTE)value,
                (DWORD)( strlen( value ) + 1 ) );
        RegCloseKey( keyHandle );
    }
}

void QVLCRegistry::WriteRegistry( const char *path, const char *valueName, double value )
{
    HKEY keyHandle;
    if( RegCreateKeyExA( m_RootKey, path, 0, NULL, REG_OPTION_NON_VOLATILE,
                       KEY_WRITE, NULL, &keyHandle, NULL ) == ERROR_SUCCESS )
    {
        RegSetValueExA( keyHandle, valueName, 0, REG_BINARY, (LPBYTE)&value, sizeof( double ) );
        RegCloseKey( keyHandle );
    }
}

int QVLCRegistry::ReadRegistry( const char *path, const char *valueName, int default_value ) {
    HKEY keyHandle;
    int tempValue;
    DWORD size1;
    DWORD valueType;

    if( RegOpenKeyExA( m_RootKey, path, 0, KEY_READ, &keyHandle ) == ERROR_SUCCESS )
    {
        if( RegQueryValueExA(  keyHandle, valueName, NULL, &valueType, NULL, &size1 ) == ERROR_SUCCESS )
        {
           if( valueType == REG_DWORD )
           {
               if( RegQueryValueExA(  keyHandle, valueName, NULL, &valueType, (LPBYTE)&tempValue, &size1 ) == ERROR_SUCCESS )
               {
                  default_value = tempValue;
               };
           }
        }
        RegCloseKey( keyHandle );
    }
    return default_value;
}

char * QVLCRegistry::ReadRegistry( const char *path, const char *valueName, const char *default_value )
{
    HKEY keyHandle;
    char *tempValue = NULL;
    char *tempValue2 = NULL;

    DWORD size1;
    DWORD valueType;

    if( RegOpenKeyExA( m_RootKey, path, 0, KEY_READ, &keyHandle ) == ERROR_SUCCESS )
    {
        if( RegQueryValueExA(  keyHandle, valueName, NULL, &valueType, NULL, &size1 ) == ERROR_SUCCESS )
        {
           if( valueType == REG_SZ )
           {
               // free
               tempValue = ( char * )malloc( size1+1 ); // +1 für NullByte`?
               if( RegQueryValueExA(  keyHandle, valueName, NULL, &valueType, (LPBYTE)tempValue, &size1 ) == ERROR_SUCCESS )
               {
                  tempValue2 = tempValue;
               };
           }
        }
        RegCloseKey( keyHandle );
    }

    return tempValue == NULL ? strdup( default_value ) : tempValue2;
}

double QVLCRegistry::ReadRegistry( const char *path, const char *valueName, double default_value )
{
    HKEY keyHandle;
    double tempValue;
    DWORD size1;
    DWORD valueType;

    if( RegOpenKeyExA( m_RootKey, path, 0, KEY_READ, &keyHandle ) == ERROR_SUCCESS )
    {
        if( RegQueryValueExA( keyHandle, valueName, NULL, &valueType,
                             NULL, &size1 ) == ERROR_SUCCESS )
        {
           if( ( valueType == REG_BINARY ) && ( size1 == sizeof( double ) ) )
           {
               if( RegQueryValueExA( keyHandle, valueName, NULL, &valueType,
                           (LPBYTE)&tempValue, &size1 ) == ERROR_SUCCESS )
               {
                  default_value = tempValue;
               };
           }
        }
        RegCloseKey( keyHandle );
    }
    return default_value;
}

int QVLCRegistry::DeleteValue( const char *path, const char *valueName )
{
    HKEY keyHandle;
    long result;
    if( (result = RegOpenKeyExA(m_RootKey, path, 0, KEY_WRITE, &keyHandle)) == ERROR_SUCCESS)
    {
        result = RegDeleteValueA(keyHandle, valueName);
        RegCloseKey(keyHandle);
    }
    //ERROR_SUCCESS = ok everything else you have a problem*g*,
    return result;
}

long QVLCRegistry::DeleteKey( const char *path, const char *keyName )
{
    HKEY keyHandle;
    long result;
    if( (result = RegOpenKeyExA(m_RootKey, path, 0, KEY_WRITE, &keyHandle)) == ERROR_SUCCESS)
    {
         // be warned the key "keyName" will not be deleted if there are subkeys below him, values
        // I think are ok and will be recusively deleted, but not keys...
        // for this case we have to do a little bit more work!
        result = RegDeleteKeyA(keyHandle, keyName);
        RegCloseKey(keyHandle);
    }
    //ERROR_SUCCESS = ok everything else you have a problem*g*,
    return result;
}

#endif /* _WIN32 */
