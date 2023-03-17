// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * test.h - libvlc smoke test common definitions
 *
 */

/**********************************************************************
 *  Copyright (C) 2007 Rémi Denis-Courmont.                           *
 *  Copyright (C) 2008 Pierre d'Herbemont.                            *
 **********************************************************************/

#ifndef TEST_H
#define TEST_H

/*********************************************************************
 * Some useful common headers
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <vlc/vlc.h>
#include <vlc_common.h>
#include <vlc_threads.h>

#undef NDEBUG
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>

/*********************************************************************
 * Some useful global var
 */

static const char * test_defaults_args[] = {
    "-v", "--vout=vdummy", "--aout=adummy", "--text-renderer=tdummy",
};

static const int test_defaults_nargs =
    sizeof (test_defaults_args) / sizeof (test_defaults_args[0]);

static const char test_default_sample[] = "mock://";

/*********************************************************************
 * Some useful common functions
 */

#define test_log( ... ) printf( "testapi: " __VA_ARGS__ );

static inline void on_timeout(int signum)
{
    assert(signum == SIGALRM);
    abort(); /* Cause a core dump */
}

static inline void test_init (void)
{
    (void)test_default_sample; /* This one may not be used */

    /* Make sure "make check" does not get stuck */
    /* Timeout of 10secs by default */
    unsigned alarm_timeout = 10;
    /* Valid timeout value are < 0, for infinite, and > 0, for the number of
     * seconds */
    char *alarm_timeout_str = getenv("VLC_TEST_TIMEOUT");
    if (alarm_timeout_str)
    {
        int val = atoi(alarm_timeout_str);
        if (val <= 0)
            alarm_timeout = 0; /* infinite */
        else
            alarm_timeout = val;
    }
    if (alarm_timeout != 0)
    {
        struct sigaction sig = {
            .sa_handler = on_timeout,
        };
        sigaction(SIGALRM, &sig, NULL);
        alarm (alarm_timeout);
    }

    setenv( "VLC_PLUGIN_PATH", "../modules", 1 );
}

#endif /* TEST_H */
