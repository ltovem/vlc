/*****************************************************************************
 * win32_dragdrop.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef WIN32_DRAGDROP_HPP
#define WIN32_DRAGDROP_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <windows.h>
#include <shellapi.h>
#include <ole2.h>
#include <list>
#include "../src/skin_common.hpp"
#include "../src/generic_window.hpp"


class Win32DragDrop: public SkinObject, public IDropTarget
{
public:
   Win32DragDrop( intf_thread_t *pIntf, GenericWindow* pWin );
   virtual ~Win32DragDrop() { }

protected:
    // IUnknown methods
    STDMETHOD(QueryInterface)( REFIID riid, void FAR* FAR* ppvObj );
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IDropTarget methods
    STDMETHOD(DragEnter)( LPDATAOBJECT pDataObj, DWORD grfKeyState,
                          POINTL pt, DWORD *pdwEffect );
    STDMETHOD(DragOver)( DWORD grfKeyState, POINTL pt, DWORD *pdwEffect );
    STDMETHOD(DragLeave)();
    STDMETHOD(Drop)( LPDATAOBJECT pDataObj, DWORD grfKeyState,
                     POINTL pt, DWORD *pdwEffect );

private:
    /// Internal reference counter
    LONG m_references;
    /// Window associated
    GenericWindow* m_pWin;
    /// format used for DrapNDrop
    struct {
        UINT format;
        const char* name;
    } m_format;
};


#endif
