/*****************************************************************************
 * os2_dragdrop.cpp
 *****************************************************************************
 * Copyright (C) 2003, 2013 the VideoLAN team
 *
 * Authors: Cyril Deguet      <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *          KO Myung-Hun      <komh@chollian.net>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifdef OS2_SKINS

#include "os2_dragdrop.hpp"
#include "../commands/cmd_add_item.hpp"
#include "../events/evt_dragndrop.hpp"
#include <list>


OS2DragDrop::OS2DragDrop( intf_thread_t *pIntf,
                              bool playOnDrop, GenericWindow* pWin )
    : SkinObject( pIntf ), m_playOnDrop( playOnDrop ), m_pWin( pWin)
{
}


#endif
