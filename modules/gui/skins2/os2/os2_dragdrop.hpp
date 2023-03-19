// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * os2_dragdrop.hpp
 *****************************************************************************
 * Copyright (C) 2003, 2013 the VideoLAN team
 *
 * Authors: Cyril Deguet      <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *          KO Myung-Hun      <komh@chollian.net>
 *****************************************************************************/

#ifndef OS2_DRAGDROP_HPP
#define OS2_DRAGDROP_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "../src/skin_common.hpp"
#include "../src/generic_window.hpp"


class OS2DragDrop: public SkinObject
{
public:
   OS2DragDrop( intf_thread_t *pIntf, bool playOnDrop, GenericWindow* pWin );
   virtual ~OS2DragDrop() { }

private:
    /// Indicates whether the file(s) must be played immediately
    bool m_playOnDrop;
    ///
    GenericWindow* m_pWin;
};


#endif
