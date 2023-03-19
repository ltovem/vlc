// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * skin_common.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef SKIN_COMMON_HPP
#define SKIN_COMMON_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_interface.h>
#include <vlc_playlist.h>
#include <vlc_player.h>
#include <vlc_charset.h>
#include <vlc_fs.h>

#include <string>
#include <memory>

class AsyncQueue;
class Logger;
class Dialogs;
class Interpreter;
class OSFactory;
class OSLoop;
class VarManager;
class VlcProc;
class VoutManager;
class ArtManager;
class Theme;
class ThemeRepository;

#ifndef M_PI
#   define M_PI 3.14159265358979323846
#endif

#ifdef _MSC_VER
// turn off 'warning C4355: 'this' : used in base member initializer list'
#pragma warning ( disable:4355 )
// turn off 'identifier was truncated to '255' characters in the debug info'
#pragma warning ( disable:4786 )
#endif

/// Wrapper around FromLocale, to avoid the need to call LocaleFree()
static inline std::string sFromLocale( const std::string &rLocale )
{
    const char *s = FromLocale( rLocale.c_str() );
    std::string res = s;
    LocaleFree( s );
    return res;
}

#ifdef _WIN32
/// Wrapper around FromWide, to avoid the need to call free()
static inline std::string sFromWide( const std::wstring &rWide )
{
    char *s = FromWide( rWide.c_str() );
    std::string res = s;
    free( s );
    return res;
}
#endif

//---------------------------------------------------------------------------
// intf_sys_t: description and status of skin interface
//---------------------------------------------------------------------------
struct intf_sys_t
{
    // "Singleton" objects: MUST be initialized to NULL !
    /// Logger
    Logger *p_logger;
    /// Asynchronous command queue
    AsyncQueue *p_queue;
    /// Dialog provider
    Dialogs *p_dialogs;
    /// Script interpreter
    Interpreter *p_interpreter;
    /// Factory for OS specific classes
    OSFactory *p_osFactory;
    /// Main OS specific message loop
    std::unique_ptr<OSLoop> p_osLoop;
    /// Variable manager
    VarManager *p_varManager;
    /// VLC state handler
    VlcProc *p_vlcProc;
    /// Vout manager
    VoutManager *p_voutManager;
    /// Art manager
    ArtManager *p_artManager;
    /// Theme repository
    ThemeRepository *p_repository;

    /// Current theme
    std::unique_ptr<Theme> p_theme;

    /// synchronisation at start of interface
    vlc_thread_t thread;
    vlc_sem_t    init_wait;
    bool         b_error;
};


/// Base class for all skin classes
class SkinObject
{
public:
    SkinObject( intf_thread_t *pIntf ): m_pIntf( pIntf ) { }
    virtual ~SkinObject() { }

    /// Getter (public because it is used in C callbacks in the win32
    /// interface)
    intf_thread_t *getIntf() const { return m_pIntf; }
    vlc_playlist_t *getPL() const
        { return vlc_intf_GetMainPlaylist( m_pIntf ); }

private:
    intf_thread_t *m_pIntf;
};


#endif
