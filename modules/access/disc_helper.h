// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * disc_helper.h: disc helper functions
 *****************************************************************************
 * Copyright (C) 2020 VLC authors and VideoLAN
 *****************************************************************************/

#ifdef __APPLE__
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

#include <vlc_dialog.h>
#include <vlc_fs.h>

inline static int DiscProbeMacOSPermission( vlc_object_t *p_this, const char *psz_file )
{
    /* Check is only relevant starting macOS Catalina */
    if( __builtin_available( macOS 10.15, * ) )
    {
        /* Continue. The check above cannot be negated. */
    }
    else
    {
        return VLC_SUCCESS;
    }

    msg_Dbg( p_this, "Checking access permission for path %s", psz_file );

    struct stat stat_buf;
    if( vlc_stat( psz_file, &stat_buf ) != 0 )
        return VLC_SUCCESS; // Continue with probing to be on the safe side

    if( !S_ISBLK( stat_buf.st_mode ) && !S_ISCHR( stat_buf.st_mode ) )
        return VLC_SUCCESS;

    /* Check that device access in fact fails with EPERM error */
    int retVal = access( psz_file, R_OK );
    if( retVal == -1 && errno == EPERM )
    {
        msg_Err( p_this, "Path %s cannot be opened due to insufficient permissions", psz_file );
        vlc_dialog_display_error( p_this, _("Problem accessing a system resource"),
            _("Potentially, macOS blocks access to your disc. "
              "Please open \"System Preferences\" -> \"Security & Privacy\" "
              "and allow VLC to access your external media in \"Files and Folders\" section."));

        return VLC_EGENERIC;
    }

    return VLC_SUCCESS;
}
#endif
