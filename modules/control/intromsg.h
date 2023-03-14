/*****************************************************************************
 * intromsg.h
 *****************************************************************************
 * Copyright (C) 1999-2015 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

static inline void intf_consoleIntroMsg(intf_thread_t *p_intf)
{
    if (getenv( "PWD" ) == NULL) /* detect Cygwin shell or Wine */
    {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);
    }

    msg_rc("VLC media player - %s", VERSION_MESSAGE);
    msg_rc("%s", COPYRIGHT_MESSAGE);
    msg_rc(_("\nWarning: if you cannot access the GUI "
                     "anymore, open a command-line window, go to the "
                     "directory where you installed VLC and run "
                     "\"vlc -I qt\"\n"));
}
