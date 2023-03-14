/*****************************************************************************
 * dvb.h : functions to control a DVB card under Linux with v4l2
 *****************************************************************************
 * Copyright (C) 1998-2005 VLC authors and VideoLAN
 *
 * Authors: Johan Bilien <jobi@via.ecp.fr>
 *          Jean-Paul Saman <jpsaman _at_ videolan _dot_ org>
 *          Christopher Ross <chris@tebibyte.org>
 *          Christophe Massiot <massiot@via.ecp.fr>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

/*****************************************************************************
 * Local structures
 *****************************************************************************/
typedef struct demux_handle_t
{
    int i_pid;
    int i_handle;
    int i_type;
} demux_handle_t;

typedef struct frontend_t frontend_t;
typedef struct
{
    int i_snr;              /**< Signal Noise ratio */
    int i_ber;              /**< Bitrate error ratio */
    int i_signal_strenth;   /**< Signal strength */
} frontend_statistic_t;

typedef struct
{
    bool b_has_signal;
    bool b_has_carrier;
    bool b_has_lock;
} frontend_status_t;

#define MAX_DEMUX 256
#define TS_PACKET_SIZE 188

struct scan_t;
struct scan_parameter_t;

typedef struct
{
    int i_handle, i_frontend_handle;
    frontend_t *p_frontend;

} dvb_sys_t;

#define VIDEO0_TYPE     1
#define AUDIO0_TYPE     2
#define TELETEXT0_TYPE  3
#define SUBTITLE0_TYPE  4
#define PCR0_TYPE       5
#define TYPE_INTERVAL   5
#define OTHER_TYPE     21

/*****************************************************************************
 * Prototypes
 *****************************************************************************/

int  FrontendOpen( vlc_object_t *, dvb_sys_t *, const char * );
void FrontendPoll( vlc_object_t *, dvb_sys_t * );
int  FrontendSet( vlc_object_t *, dvb_sys_t * );
void FrontendClose( vlc_object_t *, dvb_sys_t * );

int  FrontendGetStatistic( dvb_sys_t *, frontend_statistic_t * );
void FrontendGetStatus( dvb_sys_t *, frontend_status_t * );
int  FrontendFillScanParameter( vlc_object_t *, dvb_sys_t *, struct scan_parameter_t * );

int DMXSetFilter( vlc_object_t *, int i_pid, int * pi_fd, int i_type );
int DMXUnsetFilter( vlc_object_t *, int i_fd );

int  DVROpen( vlc_object_t *, dvb_sys_t * );
void DVRClose( vlc_object_t *, dvb_sys_t * );
