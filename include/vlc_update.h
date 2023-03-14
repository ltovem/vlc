/*****************************************************************************
 * vlc_update.h: VLC update download
 *****************************************************************************
 * Copyright © 2005-2007 VLC authors and VideoLAN
 *
 * Authors: Antoine Cellerier <dionoea -at- videolan -dot- org>
 *          Rafaël Carré <funman@videolanorg>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_UPDATE_H
#define VLC_UPDATE_H

/**
 * \defgroup update Software updates
 * \ingroup interface
 * Over-the-air VLC software updates
 * @{
 * \file
 *VLC software update interface
 */

/**
 * Describes an update VLC release number
 */
struct update_release_t
{
    int i_major;        ///< Version major
    int i_minor;        ///< Version minor
    int i_revision;     ///< Version revision
    int i_extra;        ///< Version extra
    char* psz_url;      ///< Download URL
    char* psz_desc;     ///< Release description
};

typedef struct update_release_t update_release_t;

VLC_API update_t * update_New( vlc_object_t * );
#define update_New( a ) update_New( VLC_OBJECT( a ) )
VLC_API void update_Delete( update_t * );
VLC_API void update_Check( update_t *, void (*callback)( void*, bool ), void * );
VLC_API bool update_NeedUpgrade( update_t * );
VLC_API void update_Download( update_t *, const char* );
VLC_API update_release_t* update_GetRelease( update_t * );

/**
 * @}
 */

#endif /* _VLC_UPDATE_H */
