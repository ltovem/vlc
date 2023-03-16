// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * tooltip.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teulière <ipkiss@via.ecp.fr>
 *****************************************************************************/

#ifndef TOOLTIP_HPP
#define TOOLTIP_HPP

#include "../utils/var_text.hpp"
#include "../commands/cmd_generic.hpp"

class GenericFont;
class OSTooltip;
class OSTimer;
class OSGraphics;
class UString;


class Tooltip: public SkinObject, public Observer<VarText>
{
public:
    /// Create a tooltip with the given font and delay (in milliseconds)
    Tooltip( intf_thread_t *pIntf, const GenericFont &rFont, int delay );

    virtual ~Tooltip();

    /// Draw the tooltip and show it after the delay
    void show();

    /// Hide the tooltip and cancel the timer if a tooltip is waiting for
    /// display
    void hide();

private:
    /// Font
    const GenericFont &m_rFont;
    /// Delay before popping the tooltip
    int m_delay;
    /// Timer to wait before showing the tooltip
    OSTimer *m_pTimer;
    /// OS specific tooltip window
    OSTooltip *m_pOsTooltip;
    /// Image of the tooltip
    OSGraphics *m_pImage;
    /// Position of the tooltip
    int m_xPos, m_yPos;

    /// Method called when the observed variable is modified
    virtual void onUpdate( Subject<VarText> &rVariable, void * );

    /// Display text of the tooltip
    void displayText( const UString &rText );

    /// Build m_pImage with the given text
    void makeImage( const UString &rText );

    /// Callback to show the tooltip window
    DEFINE_CALLBACK( Tooltip, Show );
};


#endif
