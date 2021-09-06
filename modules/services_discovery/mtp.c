/*****************************************************************************
 * mtp.c :  MTP interface module
 *****************************************************************************
 * Copyright (C) 2009 the VideoLAN team
 *
 * Authors: Fabio Ritrovato <exsephiroth87@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#define VLC_MODULE_LICENSE VLC_LICENSE_GPL_2_PLUS
#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_services_discovery.h>
#include <vlc_interrupt.h>

#include <sys/time.h>
#include <poll.h>
#include <errno.h>

#include <stdbool.h>
#include <libmtp.h>
#include <libusb.h>

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
static int Open( vlc_object_t * );
static void Close( vlc_object_t * );

VLC_SD_PROBE_HELPER("mtp", N_("MTP devices"), SD_CAT_DEVICES)

vlc_module_begin()
    set_shortname( "MTP" )
    set_description( N_( "MTP devices" ) )
    set_category( CAT_PLAYLIST )
    set_subcategory( SUBCAT_PLAYLIST_SD )
    set_capability( "services_discovery", 0 )
    set_callbacks( Open, Close )
    cannot_unload_broken_library()

    VLC_SD_PROBE_SUBMODULE
vlc_module_end()



/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

static void *Run( void * );
static void *RunLibUSB( void * );
int mtp_usbHotplugCallback(struct libusb_context *, struct libusb_device *, libusb_hotplug_event, void *);

static int AddDevice( services_discovery_t *, LIBMTP_raw_device_t * );
static void AddTrack( services_discovery_t *, LIBMTP_track_t *);
static void CloseDevice( services_discovery_t * );
static int CountTracks( uint64_t const, uint64_t const, void const * const );

/*****************************************************************************
 * Local structures
 *****************************************************************************/

#define HOTPLUG_CB_USB_NO_EVENT 0x0
#define HOTPLUG_CB_USB_DEV_ARRIVED 0x1
#define HOTPLUG_CB_USB_DEV_LEFT 0x2

typedef struct
{
    int i_tracks_num;
    input_item_t **pp_items;
    int i_count;
    char *psz_name;
    uint32_t i_bus;
    uint8_t i_dev;
    uint16_t i_product_id;
    vlc_thread_t run_thread;
    vlc_thread_t handle_events_thread;
    libusb_context* p_libusb_ctx;
    libusb_hotplug_callback_handle cb_handle;
    int i_hotplug_cb_status;
    bool b_hotplug_end;
    vlc_interrupt_t *interrupt_ctx;
    vlc_mutex_t mutex;
} services_discovery_sys_t;

/*****************************************************************************
 * Open: initialize and create stuff
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    services_discovery_t *p_sd = ( services_discovery_t * )p_this;
    services_discovery_sys_t *p_sys;

    if( !( p_sys = malloc( sizeof( services_discovery_sys_t ) ) ) )
        return VLC_ENOMEM;
    p_sd->p_sys = p_sys;
    p_sd->description = _("MTP devices");
    p_sys->psz_name = NULL;

    static vlc_once_t mtp_init_once = VLC_STATIC_ONCE;

    vlc_once( &mtp_init_once, LIBMTP_Init );

#if defined(__APPLE__) || defined(__linux__)
    if ( libusb_init (&p_sys->p_libusb_ctx) != LIBUSB_SUCCESS )
        return VLC_EGENERIC;
    if ( libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG) != 0 )
    {
        libusb_exit( p_sys->p_libusb_ctx );
        return VLC_EGENERIC;
    }
    p_sys->interrupt_ctx = vlc_interrupt_create();
    p_sys->i_hotplug_cb_status = HOTPLUG_CB_USB_DEV_ARRIVED;
    p_sys->b_hotplug_end = false;
    vlc_mutex_init(&p_sys->mutex);
    int rc = libusb_hotplug_register_callback( p_sys->p_libusb_ctx,
                                               LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                               LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT,
                                               0,
                                               LIBUSB_HOTPLUG_MATCH_ANY,
                                               LIBUSB_HOTPLUG_MATCH_ANY,
                                               LIBUSB_HOTPLUG_MATCH_ANY,
                                               mtp_usbHotplugCallback, p_sys,
                                               &p_sys->cb_handle );
    if ( rc != LIBUSB_SUCCESS )
    {
        libusb_exit( p_sys->p_libusb_ctx );
        free( p_sys );
        return VLC_EGENERIC;
    }
    if ( vlc_clone( &p_sys->run_thread, RunLibUSB, p_sd, VLC_THREAD_PRIORITY_LOW ) )
    {
        libusb_hotplug_deregister_callback( p_sys->p_libusb_ctx, p_sys->cb_handle );
        libusb_exit( p_sys->p_libusb_ctx );
        free( p_sys );
        return VLC_EGENERIC;
    }
    return VLC_SUCCESS;
#else
    if (vlc_clone (&p_sys->run_thread, Run, p_sd, VLC_THREAD_PRIORITY_LOW))
    {
        free (p_sys);
        return VLC_EGENERIC;
    }
    return VLC_SUCCESS;
#endif
}

/*****************************************************************************
 * Close: cleanup
 *****************************************************************************/
static void Close( vlc_object_t *p_this )
{
    services_discovery_t *p_sd = ( services_discovery_t * )p_this;
    services_discovery_sys_t *p_sys = p_sd->p_sys;
    free( p_sys->psz_name );
#if defined(__APPLE__) || defined(__linux__)
    if ( libusb_has_capability( LIBUSB_CAP_HAS_HOTPLUG ) != 0 )
    {
        vlc_mutex_lock(&p_sys->mutex);
        p_sys->b_hotplug_end = true;
        vlc_mutex_unlock(&p_sys->mutex);
        vlc_interrupt_raise( p_sys->interrupt_ctx );
        libusb_hotplug_deregister_callback( p_sys->p_libusb_ctx, p_sys->cb_handle );
    }
    vlc_join( p_sys->run_thread, NULL );
    libusb_exit( p_sys->p_libusb_ctx );
#else
    vlc_cancel( p_sys->run_thread );
    vlc_join( p_sys->run_thread, NULL );
#endif
    free( p_sys );
}


int mtp_usbHotplugCallback( struct libusb_context *p_ctx, struct libusb_device *p_dev, libusb_hotplug_event event, void *p_user_data )
{
    (void) p_ctx;
    (void) p_dev;
    services_discovery_sys_t *p_sys = ( services_discovery_sys_t * ) p_user_data;
    switch ( event )
    {
    case LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED:
        p_sys->i_hotplug_cb_status |= HOTPLUG_CB_USB_DEV_ARRIVED;
        break;
    case LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT:
        p_sys->i_hotplug_cb_status |= HOTPLUG_CB_USB_DEV_LEFT;
        break;
    }
    return 0;
}

/*****************************************************************************
 * Run: main thread
 *****************************************************************************/

#if defined(__APPLE__) || defined(__linux__)
static void *RunLibUSB ( void *data )
{
    LIBMTP_raw_device_t *p_rawdevices;
    int i_numrawdevices;
    int i_ret;
    services_discovery_t *p_sd = data;
    services_discovery_sys_t *p_sys = p_sd->p_sys;
    
    const struct libusb_pollfd ** pollfds = libusb_get_pollfds(p_sys->p_libusb_ctx);
    if(pollfds == NULL)
    {
        return NULL;
    }
    int nfds;
    for(nfds = 0; pollfds[nfds] != NULL; nfds++);
    struct timeval zero_tv = { .tv_sec = 0, .tv_usec = 0 };

    for(;;)
    {
        if ( ( vlc_poll_i11e((struct pollfd *) *pollfds, nfds, -1) == -1 ) )
            break;
        vlc_mutex_lock( &p_sys->mutex );
        bool b_end = p_sys->b_hotplug_end;
         vlc_mutex_unlock( &p_sys->mutex );
        if ( b_end )
            break;

        libusb_handle_events_timeout(p_sys->p_libusb_ctx, &zero_tv);

        if ( p_sys->i_hotplug_cb_status == HOTPLUG_CB_USB_NO_EVENT )
            continue;

        int i_status = p_sys->i_hotplug_cb_status;
        p_sys->i_hotplug_cb_status = HOTPLUG_CB_USB_NO_EVENT;

        i_ret = LIBMTP_Detect_Raw_Devices( &p_rawdevices, &i_numrawdevices );
        if ( ( i_status & HOTPLUG_CB_USB_DEV_LEFT ) &&
             ( i_ret != 0 || i_numrawdevices == 0 || p_rawdevices == NULL ) )
        {
            /* The device is not connected anymore, delete it */
            msg_Info( p_sd, "Device disconnected" );
            CloseDevice ( p_sd );
        }
        if ( ( i_status & HOTPLUG_CB_USB_DEV_ARRIVED ) &&
             i_ret == 0 && i_numrawdevices > 0 && p_rawdevices != NULL )
        {
            msg_Dbg( p_sd, "New device found" );
            AddDevice( p_sd, &p_rawdevices[0] );
        }
        free ( p_rawdevices );
    }
    
    libusb_free_pollfds(pollfds);
    return NULL;
}
#else
static void *Run( void *data )
{
    LIBMTP_raw_device_t *p_rawdevices;
    int i_numrawdevices;
    int i_ret;
    int i_status = 0;
    services_discovery_t *p_sd = data;
    for(;;)
    {
        int canc = vlc_savecancel();
        i_ret = LIBMTP_Detect_Raw_Devices( &p_rawdevices, &i_numrawdevices );
        if ( i_ret == 0 && i_numrawdevices > 0 && p_rawdevices != NULL &&
             i_status == 0 )
        {
            /* Found a new device, add it */
            msg_Dbg( p_sd, "New device found" );
            if( AddDevice( p_sd, &p_rawdevices[0] ) == VLC_SUCCESS )
                i_status = 1;
            else
                i_status = 2;
        }
        else
        {
            if ( ( i_ret != 0 || i_numrawdevices == 0 || p_rawdevices == NULL )
                 && i_status == 1)
            {
                /* The device is not connected anymore, delete it */
                msg_Info( p_sd, "Device disconnected" );
                CloseDevice( p_sd );
                i_status = 0;
            }
        }
        free( p_rawdevices );
        vlc_restorecancel(canc);
        if( i_status == 2 )
        {
            vlc_tick_sleep( VLC_TICK_FROM_SEC(5) );
            i_status = 0;
        }
        else
            vlc_tick_sleep( VLC_TICK_FROM_MS(500) );
    }
    return NULL;
}
#endif

/*****************************************************************************
 * Everything else
 *****************************************************************************/
static int AddDevice( services_discovery_t *p_sd,
                      LIBMTP_raw_device_t *p_raw_device )
{
    services_discovery_sys_t *p_sys = p_sd->p_sys;
    char *psz_name = NULL;
    LIBMTP_mtpdevice_t *p_device;
    LIBMTP_track_t *p_track, *p_tmp;

    if( ( p_device = LIBMTP_Open_Raw_Device( p_raw_device ) ) != NULL )
    {
        if( !( psz_name = LIBMTP_Get_Friendlyname( p_device ) ) )
            if( !( psz_name = LIBMTP_Get_Modelname( p_device ) ) )
                if( !( psz_name = strdup( N_( "MTP Device" ) ) ) )
                    return VLC_ENOMEM;
        msg_Info( p_sd, "Found device: %s", psz_name );
        p_sys->i_bus = p_raw_device->bus_location;
        p_sys->i_dev = p_raw_device->devnum;
        p_sys->i_product_id = p_raw_device->device_entry.product_id;
        if( ( p_track = LIBMTP_Get_Tracklisting_With_Callback( p_device,
                            CountTracks, p_sd ) ) == NULL )
        {
            msg_Warn( p_sd, "No tracks on the device" );
            p_sys->pp_items = NULL;
        }
        else
        {
            if( !( p_sys->pp_items = calloc( p_sys->i_tracks_num,
                                                   sizeof( input_item_t * ) ) ) )
            {
                free( psz_name );
                return VLC_ENOMEM;
            }
            p_sys->i_count = 0;
            while( p_track != NULL )
            {
                msg_Dbg( p_sd, "Track found: %s - %s", p_track->artist,
                         p_track->title );
                AddTrack( p_sd, p_track );
                p_tmp = p_track;
                p_track = p_track->next;
                LIBMTP_destroy_track_t( p_tmp );
            }
        }
        p_sys->psz_name = psz_name;
        LIBMTP_Release_Device( p_device );
        return VLC_SUCCESS;
    }
    else
    {
        msg_Info( p_sd, "The device seems to be mounted, unmount it first" );
        return VLC_EGENERIC;
    }
}

static void AddTrack( services_discovery_t *p_sd, LIBMTP_track_t *p_track )
{
    services_discovery_sys_t *p_sys = p_sd->p_sys;
    input_item_t *p_input;
    char *psz_string;
    char *extension;

    extension = rindex( p_track->filename, '.' );
    if( asprintf( &psz_string, "mtp://%"PRIu32":%"PRIu8":%"PRIu16":%d%s",
                  p_sys->i_bus, p_sys->i_dev,
                  p_sys->i_product_id, p_track->item_id,
                  extension ) == -1 )
    {
        msg_Err( p_sd, "Error adding %s, skipping it", p_track->filename );
        return;
    }
    if( ( p_input = input_item_New( psz_string, p_track->title ) ) == NULL )
    {
        msg_Err( p_sd, "Error adding %s, skipping it", p_track->filename );
        free( psz_string );
        return;
    }
    free( psz_string );

    input_item_SetArtist( p_input, p_track->artist );
    input_item_SetGenre( p_input, p_track->genre );
    input_item_SetAlbum( p_input, p_track->album );
    if( asprintf( &psz_string, "%d", p_track->tracknumber ) != -1 )
    {
        input_item_SetTrackNum( p_input, psz_string );
        free( psz_string );
    }
    if( asprintf( &psz_string, "%d", p_track->rating ) != -1 )
    {
        input_item_SetRating( p_input, psz_string );
        free( psz_string );
    }
    input_item_SetDate( p_input, p_track->date );
    p_input->i_duration = VLC_TICK_FROM_MS(p_track->duration);
    services_discovery_AddItem( p_sd, p_input );
    p_sys->pp_items[p_sys->i_count++] = p_input;
}

static void CloseDevice( services_discovery_t *p_sd )
{
    services_discovery_sys_t *p_sys = p_sd->p_sys;
    input_item_t **pp_items = p_sys->pp_items;

    if( pp_items != NULL )
    {
        for( int i_i = 0; i_i < p_sys->i_count; i_i++ )
        {
            if( pp_items[i_i] != NULL )
            {
                services_discovery_RemoveItem( p_sd, pp_items[i_i] );
                input_item_Release( pp_items[i_i] );
            }
        }
        free( pp_items );
    }
}

static int CountTracks( uint64_t const sent, uint64_t const total,
                        void const * const data )
{
    VLC_UNUSED( sent );
    services_discovery_t *p_sd = (services_discovery_t *)data;
    services_discovery_sys_t *p_sys = p_sd->p_sys;
    p_sys->i_tracks_num = total;
    return 0;
}
