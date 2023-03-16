// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * dlna_common.hpp : DLNA common header
 *****************************************************************************
 * Copyright © 2018 VLC authors and VideoLAN
 *
 * Authors: Shaleen Jain <shaleen@jain.sh>
 *****************************************************************************/

#ifndef DLNA_COMMON_H
#define DLNA_COMMON_H

#include <list>

#include <vlc_common.h>
#include <vlc_fourcc.h>

#include "../renderer_common.hpp"

#define SOUT_CFG_PREFIX "sout-dlna-"

namespace DLNA
{

/* module callbacks */
int OpenSout(vlc_object_t *);
void CloseSout(vlc_object_t *);

}
#endif /* DLNA_COMMON_H */
