// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * evt_mouse.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef EVT_MOUSE_HPP
#define EVT_MOUSE_HPP

#include "evt_input.hpp"


/// Class for mouse button events
class EvtMouse: public EvtInput
{
public:
    enum ButtonType_t
    {
        kLeft,
        kMiddle,
        kRight
    };

    enum ActionType_t
    {
        kDown,
        kUp,
        kDblClick
    };

    EvtMouse( intf_thread_t *pIntf, int xPos, int yPos, ButtonType_t button,
              ActionType_t action, int mod = kModNone )
            : EvtInput( pIntf, mod ), m_xPos( xPos ), m_yPos( yPos ),
              m_button( button ), m_action( action ) { }
    virtual ~EvtMouse() { }
    virtual const std::string getAsString() const;

    // Return the event coordinates
    int getXPos() const { return m_xPos; }
    int getYPos() const { return m_yPos; }

    // Return the button and the action
    ButtonType_t getButton() const { return m_button; }
    ActionType_t getAction() const { return m_action; }

private:
    /// Coordinates of the mouse relative to the window
    int m_xPos, m_yPos;
    /// Mouse button involved in the event
    ButtonType_t m_button;
    /// Type of action
    ActionType_t m_action;
};


#endif
