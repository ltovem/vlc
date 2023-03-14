/*****************************************************************************
 * freeaddrinfo.c: freeaddrinfo() replacement functions
 *****************************************************************************
 * Copyright (C) 2005 the VideoLAN team
 * Copyright (C) 2002-2007 Rémi Denis-Courmont
 * Copyright (C) 2011-2014 KO Myung-Hun
 *
 * Authors: KO Myung-Hun <komh@chollian.net>
 *          Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>

/*
 * This function must be used to free the memory allocated by getaddrinfo().
 */
void freeaddrinfo (struct addrinfo *res)
{
    while (res != NULL)
    {
        struct addrinfo *next = res->ai_next;

        free (res->ai_canonname);
        free (res->ai_addr);
        free (res);
        res = next;
    }
}
