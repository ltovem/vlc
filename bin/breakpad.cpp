// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * breakpad.cpp: Wrapper to breakpad crash handler
 *****************************************************************************
 * Copyright (C) 1998-2017 VLC authors
 *
 * Authors: Hugo Beauzée-Luyssen <hugo@beauzee.fr>
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <windows.h>
#include "client/windows/handler/exception_handler.h"
#include "common/windows/http_upload.h"
#include <memory>
#include <map>
#include <string>

using google_breakpad::ExceptionHandler;

static bool FilterCallback(void*, EXCEPTION_POINTERS*, MDRawAssertionInfo*)
{
    // Don't spam breakpad if we're debugging
    return !IsDebuggerPresent();
}

extern "C"
{

void CheckCrashDump( const wchar_t* path )
{
    wchar_t pattern[MAX_PATH];
    WIN32_FIND_DATA data;
    _snwprintf( pattern, MAX_PATH, L"%s/*.dmp", path );
    HANDLE h = FindFirstFile( pattern, &data );
    if (h == INVALID_HANDLE_VALUE)
        return;
    int answer = MessageBox( NULL, L"Ooops: VLC media player just crashed.\n" \
        "Would you like to send a bug report to the developers team?",
        L"VLC crash reporting", MB_YESNO);
    std::map<std::wstring, std::wstring> params;
    params[L"prod"] = L"VLC";
    params[L"ver"] = TEXT(PACKAGE_VERSION);
    do
    {
        wchar_t fullPath[MAX_PATH];
        _snwprintf( fullPath, MAX_PATH, L"%s/%s", path, data.cFileName );
        if( answer == IDYES )
        {
            std::map<std::wstring, std::wstring> files;
            files[L"upload_file_minidump"] = fullPath;
            google_breakpad::HTTPUpload::SendRequest(
                            TEXT( BREAKPAD_URL "/reports" ), params, files,
                            NULL, NULL, NULL );
        }
        DeleteFile( fullPath );
    } while ( FindNextFile( h, &data ) );
    FindClose(h);
}

void* InstallCrashHandler( const wchar_t* crashdump_path )
{
    // Breakpad needs the folder to exist to generate the crashdump
    CreateDirectory( crashdump_path, NULL );
    return new(std::nothrow) ExceptionHandler( crashdump_path, FilterCallback,
                                NULL, NULL, ExceptionHandler::HANDLER_ALL);
}

void ReleaseCrashHandler( void* handler )
{
    ExceptionHandler* eh = reinterpret_cast<ExceptionHandler*>( handler );
    delete eh;
}

}
