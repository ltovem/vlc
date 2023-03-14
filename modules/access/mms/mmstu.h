/*****************************************************************************
 * mmstu.h: MMS access plug-in
 *****************************************************************************
 * Copyright (C) 2001, 2002 VLC authors and VideoLAN
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef VLC_MMS_MMSTU_H_
#define VLC_MMS_MMSTU_H_

#define MMS_PACKET_ANY          0
#define MMS_PACKET_CMD          1
#define MMS_PACKET_HEADER       2
#define MMS_PACKET_MEDIA        3
#define MMS_PACKET_UDP_TIMING   4

#define MMS_CMD_HEADERSIZE  48

#define MMS_BUFFER_SIZE 100000

typedef struct
{
    int         i_proto;        /* MMS_PROTO_TCP, MMS_PROTO_UDP */
    int         i_handle_tcp;   /* TCP socket for communication with server */
    int         i_handle_udp;   /* Optional UDP socket for data(media/header packet) */
                                /* send by server */
    char        sz_bind_addr[NI_MAXNUMERICHOST]; /* used by udp */

    vlc_url_t   url;
    uint64_t    i_position;
    uint64_t    i_size;

    asf_header_t    asfh;

    unsigned    i_timeout;

    /* */
    uint8_t     buffer_tcp[MMS_BUFFER_SIZE];
    size_t      i_buffer_tcp;

    uint8_t     buffer_udp[MMS_BUFFER_SIZE];
    size_t      i_buffer_udp;

    /* data necessary to send data to server */
    vlc_guid_t  guid;
    int         i_command_level;
    int         i_seq_num;
    uint32_t    i_header_packet_id_type;
    uint32_t    i_media_packet_id_type;

    int         i_packet_seq_num;

    uint8_t     *p_cmd;     /* latest command read */
    size_t      i_cmd;      /* allocated at the beginning */

    uint8_t     *p_header;  /* allocated by mms_ReadPacket */
    size_t      i_header;

    uint8_t     *p_media;   /* allocated by mms_ReadPacket */
    size_t      i_media;
    size_t      i_media_used;

    /* extracted information */
    int         i_command;

    /* from 0x06 answer */
    uint32_t    i_flags_broadcast;
    uint32_t    i_media_length;
    size_t      i_packet_length;
    uint32_t    i_packet_count;
    uint32_t    i_max_bit_rate;
    size_t      i_header_size;

    /* misc */
    bool  b_seekable;
    bool  b_eof;

    vlc_mutex_t  lock_netwrite;
    bool         b_keep_alive;
    struct {
         vlc_thread_t thread;
         vlc_sem_t    sem;
    } keep_alive;
} access_sys_t;

#endif
