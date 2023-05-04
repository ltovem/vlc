/*****************************************************************************
 * libvlc_media.h:  libvlc external API
 *****************************************************************************
 * Copyright (C) 1998-2009 VLC authors and VideoLAN
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Paul Saman <jpsaman@videolan.org>
 *          Pierre d'Herbemont <pdherbemont@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef VLC_LIBVLC_MEDIA_H
#define VLC_LIBVLC_MEDIA_H 1

#include <vlc/libvlc_picture.h>
#include <vlc/libvlc_media_track.h>

# ifdef __cplusplus
extern "C" {
# else
#  include <stdbool.h>
# endif
#include <stddef.h>

/** \defgroup libvlc_media LibVLC media
 * \ingroup libvlc
 * @ref libvlc_media_t is an abstract representation of a playable media.
 * It consists of a media location and various optional meta data.
 * @{
 * \file
 * LibVLC media item/descriptor external API
 */

typedef struct libvlc_media_t libvlc_media_t;

/** Meta data types */
typedef enum libvlc_meta_t {
    libvlc_meta_Title,
    libvlc_meta_Artist,
    libvlc_meta_Genre,
    libvlc_meta_Copyright,
    libvlc_meta_Album,
    libvlc_meta_TrackNumber,
    libvlc_meta_Description,
    libvlc_meta_Rating,
    libvlc_meta_Date,
    libvlc_meta_Setting,
    libvlc_meta_URL,
    libvlc_meta_Language,
    libvlc_meta_NowPlaying,
    libvlc_meta_Publisher,
    libvlc_meta_EncodedBy,
    libvlc_meta_ArtworkURL,
    libvlc_meta_TrackID,
    libvlc_meta_TrackTotal,
    libvlc_meta_Director,
    libvlc_meta_Season,
    libvlc_meta_Episode,
    libvlc_meta_ShowName,
    libvlc_meta_Actors,
    libvlc_meta_AlbumArtist,
    libvlc_meta_DiscNumber,
    libvlc_meta_DiscTotal
    /* Add new meta types HERE */
} libvlc_meta_t;

/**
 * libvlc media or media_player state
 */
typedef enum libvlc_state_t
{
    libvlc_NothingSpecial=0,
    libvlc_Opening,
    libvlc_Playing,
    libvlc_Paused,
    libvlc_Stopped,
    libvlc_Stopping,
    libvlc_Error
} libvlc_state_t;

enum
{
    libvlc_media_option_trusted = 0x2,
    libvlc_media_option_unique = 0x100
};

typedef struct libvlc_media_stats_t
{
    /* Input */
    uint64_t     i_read_bytes;
    float       f_input_bitrate;

    /* Demux */
    uint64_t     i_demux_read_bytes;
    float       f_demux_bitrate;
    uint64_t     i_demux_corrupted;
    uint64_t     i_demux_discontinuity;

    /* Decoders */
    uint64_t     i_decoded_video;
    uint64_t     i_decoded_audio;

    /* Video Output */
    uint64_t     i_displayed_pictures;
    uint64_t     i_late_pictures;
    uint64_t     i_lost_pictures;

    /* Audio output */
    uint64_t     i_played_abuffers;
    uint64_t     i_lost_abuffers;
} libvlc_media_stats_t;

/**
 * Media type
 *
 * \see libvlc_media_get_type
 */
typedef enum libvlc_media_type_t {
    libvlc_media_type_unknown,
    libvlc_media_type_file,
    libvlc_media_type_directory,
    libvlc_media_type_disc,
    libvlc_media_type_stream,
    libvlc_media_type_playlist,
} libvlc_media_type_t;

/**
 * Parse status used sent by libvlc_media_parse_request() or returned by
 * libvlc_media_get_parsed_status()
 */
typedef enum libvlc_media_parsed_status_t
{
    libvlc_media_parsed_status_none,
    libvlc_media_parsed_status_pending,
    libvlc_media_parsed_status_skipped,
    libvlc_media_parsed_status_failed,
    libvlc_media_parsed_status_timeout,
    libvlc_media_parsed_status_done,
} libvlc_media_parsed_status_t;
/**
 * Type of a media slave: subtitle or audio.
 */
typedef enum libvlc_media_slave_type_t
{
    libvlc_media_slave_type_subtitle,
    libvlc_media_slave_type_generic,
    libvlc_media_slave_type_audio = libvlc_media_slave_type_generic,
} libvlc_media_slave_type_t;

/**
 * A slave of a libvlc_media_t
 * \see libvlc_media_slaves_get
 */
typedef struct libvlc_media_slave_t
{
    char *                          psz_uri;
    libvlc_media_slave_type_t       i_type;
    unsigned int                    i_priority;
} libvlc_media_slave_t;

/**
 * Type of stat that can be requested from libvlc_media_get_filestat()
 */
#define libvlc_media_filestat_mtime 0
#define libvlc_media_filestat_size 1

/**
 * Initial version of the struct defining callbacks for
 * libvlc_media_new_callbacks()
 */
#define LIBVLC_MEDIA_OPEN_CBS_VER_0 0

/**
 * Last version of the struct defining callbacks for
 * libvlc_media_new_callbacks()
 */
#define LIBVLC_MEDIA_OPEN_CBS_VER_LATEST \
    LIBVLC_MEDIA_OPEN_CBS_VER_0

/**
 * struct defining callbacks for libvlc_media_new_callbacks()
 */
struct libvlc_media_open_cbs
{
    /**
     * Callback prototype to open a custom bitstream input media.
     *
     * The same media item can be opened multiple times. Each time, this callback
     * is invoked. It should allocate and initialize any instance-specific
     * resources, then store them in *datap. The instance resources can be freed
     * in the @ref close callback.
     *
     * \param cbs_opaque private pointer as passed to libvlc_media_new_callbacks()
     * \param datap storage space for a private data pointer [OUT]
     * \param sizep byte length of the bitstream or UINT64_MAX if unknown [OUT]
     *
     * \note For convenience, *datap is initially NULL and *sizep is initially 0.
     *
     * \note Optional (can be NULL),
     * available since \ref LIBVLC_MEDIA_OPEN_CBS_VER_LATEST
     *
     * \note If NULL, the opaque pointer will be passed to read_cb,
     * seek_cb and close_cb, and the stream size will be treated as unknown.
     *
     * \return 0 on success, non-zero on error. In case of failure, the other
     * callbacks will not be invoked and any value stored in *datap and *sizep is
     * discarded.
     */
    int (*open)(void *cbs_opaque, void **datap, uint64_t *sizep);

    /**
     * Callback prototype to read data from a custom bitstream input media.
     *
     * \param data private pointer as set by the @ref open callback
     * \param buf start address of the buffer to read data into
     * \param len bytes length of the buffer
     *
     * \note Mandatory (can't be NULL),
     * available since \ref LIBVLC_MEDIA_OPEN_CBS_VER_LATEST
     *
     * \return strictly positive number of bytes read, 0 on end-of-stream,
     *         or -1 on non-recoverable error
     *
     * \note If no data is immediately available, then the callback should sleep.
     * \warning The application is responsible for avoiding deadlock situations.
     */
    ptrdiff_t (*read)(void *data, unsigned char *buf, size_t len);

    /**
     * Callback prototype to seek a custom bitstream input media.
     *
     * \note Optional (can be NULL if seeking is not supported),
     * available since \ref LIBVLC_MEDIA_OPEN_CBS_VER_LATEST
     *
     * \param data private pointer as set by the @ref open callback
     * \param offset absolute byte offset to seek to
     * \return 0 on success, -1 on error.
     */
    int (*seek)(void *data, uint64_t offset);

    /**
     * Callback prototype to close a custom bitstream input media.
     *
     * \note Optional (can be NULL),
     * available since \ref LIBVLC_MEDIA_OPEN_CBS_VER_LATEST
     *
     * \param data private pointer as set by the @ref open callback
     */
    void (*close)(void *data);
};

/**
 * Create a media with a certain given media resource location,
 * for instance a valid URL.
 *
 * \note To refer to a local file with this function,
 * the file://... URI syntax <b>must</b> be used (see IETF RFC3986).
 * We recommend using libvlc_media_new_path() instead when dealing with
 * local files.
 *
 * \see libvlc_media_release
 *
 * \param psz_mrl the media location
 * \return the newly created media or NULL on error
 */
LIBVLC_API libvlc_media_t *libvlc_media_new_location(const char * psz_mrl);

/**
 * Create a media for a certain file path.
 *
 * \see libvlc_media_release
 *
 * \param path local filesystem path
 * \return the newly created media or NULL on error
 */
LIBVLC_API libvlc_media_t *libvlc_media_new_path(const char *path);

/**
 * Create a media for an already open file descriptor.
 * The file descriptor shall be open for reading (or reading and writing).
 *
 * Regular file descriptors, pipe read descriptors and character device
 * descriptors (including TTYs) are supported on all platforms.
 * Block device descriptors are supported where available.
 * Directory descriptors are supported on systems that provide fdopendir().
 * Sockets are supported on all platforms where they are file descriptors,
 * i.e. all except Windows.
 *
 * \note This library will <b>not</b> automatically close the file descriptor
 * under any circumstance. Nevertheless, a file descriptor can usually only be
 * rendered once in a media player. To render it a second time, the file
 * descriptor should probably be rewound to the beginning with lseek().
 *
 * \see libvlc_media_release
 *
 * \version LibVLC 1.1.5 and later.
 *
 * \param fd open file descriptor
 * \return the newly created media or NULL on error
 */
LIBVLC_API libvlc_media_t *libvlc_media_new_fd(int fd);

/**
 * Create a media with custom callbacks to read the data from.
 *
 * \param cbs_version version of the struct defining callbacks, should be
 * \ref LIBVLC_MEDIA_OPEN_CBS_VER_LATEST
 * \param cbs callback to setup the media (can't be NULL)
 * \param cbs_opaque opaque pointer for the open callback
 *
 * \return the newly created media or NULL on error
 *
 * \note The callbacks may be called asynchronously (from another thread).
 * A single stream instance need not be reentrant. However the open_cb needs to
 * be reentrant if the media is used by multiple player instances.
 *
 * \warning The callbacks may be used until all or any player instances
 * that were supplied the media item are stopped.
 *
 * \warning The function prototype changed between LibVLC 3.0.0 and LibVLC 4.0.0
 *
 * \see libvlc_media_release
 *
 * \version LibVLC 3.0.0 and later
 */
LIBVLC_API libvlc_media_t *
libvlc_media_new_callbacks(unsigned cbs_version,
                           const struct libvlc_media_open_cbs *cbs,
                           void *cbs_opaque);

/**
 * Create a media as an empty node with a given name.
 *
 * \see libvlc_media_release
 *
 * \param psz_name the name of the node
 * \return the new empty media or NULL on error
 */
LIBVLC_API libvlc_media_t *libvlc_media_new_as_node(const char * psz_name);

/**
 * Add an option to the media.
 *
 * This option will be used to determine how the media_player will
 * read the media. This allows to use VLC's advanced
 * reading/streaming options on a per-media basis.
 *
 * \note The options are listed in 'vlc --longhelp' from the command line,
 * e.g. "--sout-all". Keep in mind that available options and their semantics
 * vary across LibVLC versions and builds.
 * \warning Not all options affects libvlc_media_t objects:
 * Specifically, due to architectural issues most audio and video options,
 * such as text renderer options, have no effects on an individual media.
 * These options must be set through libvlc_new() instead.
 *
 * \param p_md the media descriptor
 * \param psz_options the options (as a string)
 */
LIBVLC_API void libvlc_media_add_option(
                                   libvlc_media_t *p_md,
                                   const char * psz_options );

/**
 * Add an option to the media with configurable flags.
 *
 * This option will be used to determine how the media_player will
 * read the media. This allows to use VLC's advanced
 * reading/streaming options on a per-media basis.
 *
 * The options are detailed in vlc --longhelp, for instance
 * "--sout-all". Note that all options are not usable on medias:
 * specifically, due to architectural issues, video-related options
 * such as text renderer options cannot be set on a single media. They
 * must be set on the whole libvlc instance instead.
 *
 * \param p_md the media descriptor
 * \param psz_options the options (as a string)
 * \param i_flags the flags for this option
 */
LIBVLC_API void libvlc_media_add_option_flag(
                                   libvlc_media_t *p_md,
                                   const char * psz_options,
                                   unsigned i_flags );


/**
 * Retain a reference to a media descriptor object (libvlc_media_t). Use
 * libvlc_media_release() to decrement the reference count of a
 * media descriptor object.
 *
 * \param p_md the media descriptor
 * \return the same object
 */
LIBVLC_API libvlc_media_t *libvlc_media_retain( libvlc_media_t *p_md );

/**
 * Decrement the reference count of a media descriptor object. If the
 * reference count is 0, then libvlc_media_release() will release the
 * media descriptor object. If the media descriptor object has been released it
 * should not be used again.
 *
 * \param p_md the media descriptor
 */
LIBVLC_API void libvlc_media_release( libvlc_media_t *p_md );


/**
 * Get the media resource locator (mrl) from a media descriptor object
 *
 * \param p_md a media descriptor object
 * \return string with mrl of media descriptor object
 */
LIBVLC_API char *libvlc_media_get_mrl( libvlc_media_t *p_md );

/**
 * Duplicate a media descriptor object.
 *
 * \warning the duplicated media won't share forthcoming updates from the
 * original one.
 *
 * \param p_md a media descriptor object.
 */
LIBVLC_API libvlc_media_t *libvlc_media_duplicate( libvlc_media_t *p_md );

/**
 * Read the meta of the media.
 *
 * Note, you need to call libvlc_media_parse_request() or play the media
 * at least once before calling this function.
 * If the media has not yet been parsed this will return NULL.
 *
 * \see libvlc_MediaMetaChanged
 *
 * \param p_md the media descriptor
 * \param e_meta the meta to read
 * \return the media's meta
 */
LIBVLC_API char *libvlc_media_get_meta( libvlc_media_t *p_md,
                                             libvlc_meta_t e_meta );

/**
 * Set the meta of the media (this function will not save the meta, call
 * libvlc_media_save_meta in order to save the meta)
 *
 * \param p_md the media descriptor
 * \param e_meta the meta to write
 * \param psz_value the media's meta
 */
LIBVLC_API void libvlc_media_set_meta( libvlc_media_t *p_md,
                                           libvlc_meta_t e_meta,
                                           const char *psz_value );

/**
 * Read the meta extra of the media.
 *
 * If the media has not yet been parsed this will return NULL.
 *
 * \see libvlc_media_parse
 * \see libvlc_media_parse_with_options
 *
 * \param p_md the media descriptor
 * \param psz_name the meta extra to read (nonnullable)
 * \return the media's meta extra or NULL
 */
LIBVLC_API char *libvlc_media_get_meta_extra( libvlc_media_t *p_md,
                                              const char *psz_name );

/**
 * Set the meta of the media (this function will not save the meta, call
 * libvlc_media_save_meta in order to save the meta)
 *
 * \param p_md the media descriptor
 * \param psz_name the meta extra to write (nonnullable)
 * \param psz_value the media's meta extra (nullable)
 * Removed from meta extra if set to NULL
 */
LIBVLC_API void libvlc_media_set_meta_extra( libvlc_media_t *p_md,
                                             const char *psz_name,
                                             const char *psz_value );

/**
 * Read the meta extra names of the media.
 *
 * \param p_md the media descriptor
 * \param pppsz_names the media's meta extra name array
 * you can access the elements using the return value (count)
 * must be released with libvlc_media_meta_extra_names_release()
 * \return the meta extra count
 */
LIBVLC_API unsigned libvlc_media_get_meta_extra_names( libvlc_media_t *p_md,
                                                       char ***pppsz_names );

/**
 * Release a media meta extra names
 *
 * \param ppsz_names meta extra names array to release
 * \param i_count number of elements in the array
 */
LIBVLC_API void libvlc_media_meta_extra_names_release( char **ppsz_names,
                                                       unsigned i_count );

/**
 * Save the meta previously set
 *
 * \param inst LibVLC instance
 * \param p_md the media descriptor
 * \return true if the write operation was successful
 */
LIBVLC_API int libvlc_media_save_meta( libvlc_instance_t *inst,
                                       libvlc_media_t *p_md );

/**
 * Get the current statistics about the media
 * \param p_md media descriptor object
 * \param p_stats structure that contain the statistics about the media
 *                 (this structure must be allocated by the caller)
 * \retval true statistics are available
 * \retval false otherwise
 */
LIBVLC_API bool libvlc_media_get_stats(libvlc_media_t *p_md,
                                       libvlc_media_stats_t *p_stats);

/* The following method uses libvlc_media_list_t, however, media_list usage is optional
 * and this is here for convenience */
#define VLC_FORWARD_DECLARE_OBJECT(a) struct a

/**
 * Get subitems of media descriptor object. This will increment
 * the reference count of supplied media descriptor object. Use
 * libvlc_media_list_release() to decrement the reference counting.
 *
 * \param p_md media descriptor object
 * \return list of media descriptor subitems or NULL
 */
LIBVLC_API VLC_FORWARD_DECLARE_OBJECT(libvlc_media_list_t *)
libvlc_media_subitems( libvlc_media_t *p_md );

/**
 * Get duration (in ms) of media descriptor object item.
 *
 * Note, you need to call libvlc_media_parse_request() or play the media
 * at least once before calling this function.
 * Not doing this will result in an undefined result.
 *
 * \param p_md media descriptor object
 * \return duration of media item or -1 on error
 */
LIBVLC_API libvlc_time_t
   libvlc_media_get_duration( libvlc_media_t *p_md );

/**
 * Get a 'stat' value of media descriptor object item.
 *
 * \note 'stat' values are currently only parsed by directory accesses. This
 * mean that only sub medias of a directory media, parsed with
 * libvlc_media_parse_request() can have valid 'stat' properties.
 * \version LibVLC 4.0.0 and later.
 *
 * \param p_md media descriptor object
 * \param type a valid libvlc_media_stat_ define
 * \param out field in which the value will be stored
 * \return 1 on success, 0 if not found, -1 on error.
 */
LIBVLC_API int
   libvlc_media_get_filestat( libvlc_media_t *p_md, unsigned type, uint64_t *out );

/**
 * Get Parsed status for media descriptor object.
 *
 * \see libvlc_MediaParsedChanged
 * \see libvlc_media_parsed_status_t
 * \see libvlc_media_parse_request()
 *
 * \param p_md media descriptor object
 * \return a value of the libvlc_media_parsed_status_t enum
 * \version LibVLC 3.0.0 or later
 */
LIBVLC_API libvlc_media_parsed_status_t
   libvlc_media_get_parsed_status( libvlc_media_t *p_md );

/**
 * Sets media descriptor's user_data. user_data is specialized data
 * accessed by the host application, VLC.framework uses it as a pointer to
 * an native object that references a libvlc_media_t pointer
 *
 * \param p_md media descriptor object
 * \param p_new_user_data pointer to user data
 */
LIBVLC_API void
    libvlc_media_set_user_data( libvlc_media_t *p_md, void *p_new_user_data );

/**
 * Get media descriptor's user_data. user_data is specialized data
 * accessed by the host application, VLC.framework uses it as a pointer to
 * an native object that references a libvlc_media_t pointer
 *
 * \see libvlc_media_set_user_data
 *
 * \param p_md media descriptor object
 */
LIBVLC_API void *libvlc_media_get_user_data( libvlc_media_t *p_md );

/**
 * Get the track list for one type
 *
 * \version LibVLC 4.0.0 and later.
 *
 * \note You need to call libvlc_media_parse_request() or play the media
 * at least once before calling this function.  Not doing this will result in
 * an empty list.
 *
 * \see libvlc_media_tracklist_count
 * \see libvlc_media_tracklist_at
 *
 * \param p_md media descriptor object
 * \param type type of the track list to request
 *
 * \return a valid libvlc_media_tracklist_t or NULL in case of error, if there
 * is no track for a category, the returned list will have a size of 0, delete
 * with libvlc_media_tracklist_delete()
 */
LIBVLC_API libvlc_media_tracklist_t *
libvlc_media_get_tracklist( libvlc_media_t *p_md, libvlc_track_type_t type );

/**
 * Get codec description from media elementary stream
 *
 * Note, you need to call libvlc_media_parse_request() or play the media
 * at least once before calling this function.
 *
 * \version LibVLC 3.0.0 and later.
 *
 * \see libvlc_media_track_t
 *
 * \param i_type i_type from libvlc_media_track_t
 * \param i_codec i_codec or i_original_fourcc from libvlc_media_track_t
 *
 * \return codec description
 */
LIBVLC_API
const char *libvlc_media_get_codec_description( libvlc_track_type_t i_type,
                                                uint32_t i_codec );

/**
 * Get the media type of the media descriptor object
 *
 * \version LibVLC 3.0.0 and later.
 *
 * \see libvlc_media_type_t
 *
 * \param p_md media descriptor object
 *
 * \return media type
 */
LIBVLC_API
libvlc_media_type_t libvlc_media_get_type( libvlc_media_t *p_md );

/**
 * Add a slave to the current media.
 *
 * A slave is an external input source that may contains an additional subtitle
 * track (like a .srt) or an additional audio track (like a .ac3).
 *
 * \note This function must be called before the media is parsed (via
 * libvlc_media_parse_request()) or before the media is played (via
 * libvlc_media_player_play())
 *
 * \version LibVLC 3.0.0 and later.
 *
 * \param p_md media descriptor object
 * \param i_type subtitle or audio
 * \param i_priority from 0 (low priority) to 4 (high priority)
 * \param psz_uri Uri of the slave (should contain a valid scheme).
 *
 * \return 0 on success, -1 on error.
 */
LIBVLC_API
int libvlc_media_slaves_add( libvlc_media_t *p_md,
                             libvlc_media_slave_type_t i_type,
                             unsigned int i_priority,
                             const char *psz_uri );

/**
 * Clear all slaves previously added by libvlc_media_slaves_add() or
 * internally.
 *
 * \version LibVLC 3.0.0 and later.
 *
 * \param p_md media descriptor object
 */
LIBVLC_API
void libvlc_media_slaves_clear( libvlc_media_t *p_md );

/**
 * Get a media descriptor's slave list
 *
 * The list will contain slaves parsed by VLC or previously added by
 * libvlc_media_slaves_add(). The typical use case of this function is to save
 * a list of slave in a database for a later use.
 *
 * \version LibVLC 3.0.0 and later.
 *
 * \see libvlc_media_slaves_add
 *
 * \param p_md media descriptor object
 * \param ppp_slaves address to store an allocated array of slaves (must be
 * freed with libvlc_media_slaves_release()) [OUT]
 *
 * \return the number of slaves (zero on error)
 */
LIBVLC_API
unsigned int libvlc_media_slaves_get( libvlc_media_t *p_md,
                                      libvlc_media_slave_t ***ppp_slaves );

/**
 * Release a media descriptor's slave list
 *
 * \version LibVLC 3.0.0 and later.
 *
 * \param pp_slaves slave array to release
 * \param i_count number of elements in the array
 */
LIBVLC_API
void libvlc_media_slaves_release( libvlc_media_slave_t **pp_slaves,
                                  unsigned int i_count );

/** @}*/

# ifdef __cplusplus
}
# endif

#include <vlc/libvlc_parser.h>

#endif /* VLC_LIBVLC_MEDIA_H */
