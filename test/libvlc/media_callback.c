/*****************************************************************************
 * imem.c: test for the imem libvlc code
 *****************************************************************************
 * Copyright (C) 2023 VideoLabs
 *
 * Authors: Alexandre Janniaux <ajanni@videolabs.io>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* Define a builtin module for mocked parts */
#define MODULE_NAME test_imem
#undef VLC_DYNAMIC_PLUGIN

#include "./test.h"
#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_stream.h>
#include <vlc_threads.h>
#include <vlc_interrupt.h>

#include <vlc/libvlc_media.h>

#include <limits.h>

#include "media_player.h"

const char vlc_module_name[] = MODULE_STRING;

struct imem_root
{
    /* root controlled semaphores */
    vlc_sem_t available;
    vlc_sem_t done;
    vlc_sem_t opened;
    vlc_sem_t read_blocking;

    vlc_sem_t wait;
};

struct imem_access
{
    struct imem_root *root;
    vlc_sem_t wait;
};

static void AccessClose(void *opaque)
{
    struct imem_access *sys = opaque;
    fprintf(stderr, "test: Access: Close\n");
    vlc_sem_post(&sys->root->done);
    vlc_sem_post(&sys->root->available);
    free(sys);
}

static int AccessOpen(void *opaque, void **datap, uint64_t *sizep)
{
    (void)sizep;
    struct imem_root *root = opaque;
    vlc_sem_wait(&root->available);
    fprintf(stderr, "test: Access: Opening new instance\n");

    struct imem_access *sys = *datap = malloc(sizeof *sys);
    if (sys == NULL)
        return -ENOMEM;

    sys->root = root;
    vlc_sem_init(&sys->wait, 0);
    vlc_sem_post(&sys->root->opened);
    return VLC_SUCCESS;
}

static ssize_t AccessRead(void *opaque, unsigned char *buf, size_t len)
{
    (void)opaque;
    assert(len < SSIZE_MAX);
    memset(buf, 0xff, len);
    return len;
}

static void SetFlag(void *opaque)
{
    bool *flag = opaque;
    *flag = true;
}

static ssize_t AccessReadBlocking(void *opaque, unsigned char *buf, size_t len)
{
    (void)opaque; (void)buf;
    struct imem_access *sys = opaque;
    assert(len < SSIZE_MAX);

    vlc_sem_post(&sys->root->read_blocking);
    /* The interruption is used to detect when the input has been closed. */
    bool was_interrupted = false;
    vlc_interrupt_register(SetFlag, &was_interrupted);
    fprintf(stderr, "test: Access: read() -> blocking\n");
    vlc_sem_wait(&sys->root->wait);
    fprintf(stderr, "test: Access: read() -> unblocked\n");
    vlc_interrupt_unregister();
    //assert(was_interrupted);

    /* We notify that we don't have data to read right now. */
    return 0;
}

static void UnblockRead(void *opaque, libvlc_media_t *media)
{
    (void)media;
    fprintf(stderr, "test: Player: Unblock read\n");
    struct imem_root *imem = opaque;
    vlc_sem_post(&imem->wait);
}

static struct imem_root *imem_root_New(void)
{

    struct imem_root *imem = malloc(sizeof *imem);
    assert(imem != NULL);

    vlc_sem_init(&imem->available, 1);
    vlc_sem_init(&imem->done, 0);
    vlc_sem_init(&imem->wait, 0);
    vlc_sem_init(&imem->opened, 0);
    vlc_sem_init(&imem->read_blocking, 0);

    return imem;
}

static void test_media_callback(libvlc_instance_t *vlc)
{
    struct imem_root *imem = imem_root_New();

    fprintf(stderr, "test: 1/ checking that media can terminate\n");
    static const struct libvlc_media_open_cbs cbs = {
        .open = AccessOpen,
        .read = AccessRead,
        .close = AccessClose,
    };
    static const struct libvlc_media_player_cbs player_cbs = {
        .on_state_changed = mp_on_state_changed,
    };
    libvlc_media_t *media;

    media = libvlc_media_new_callbacks(LIBVLC_MEDIA_OPEN_CBS_VER_LATEST,
                                       &cbs, imem);
    assert(media != NULL);

    struct mp_event_ctx ctx;
    mp_event_ctx_init(&ctx);
    libvlc_media_player_t *player =
        libvlc_media_player_new(vlc, LIBVLC_MEDIA_PLAYER_CBS_VER_LATEST,
                                &player_cbs, &ctx);
    assert(player != NULL);

    libvlc_media_player_set_media(player, media);
    libvlc_media_player_play(player);

    mp_event_ctx_wait_state(&ctx, libvlc_Opening);
    mp_event_ctx_wait_state(&ctx, libvlc_Playing);

    libvlc_media_player_stop_async(player);

    mp_event_ctx_wait_state(&ctx, libvlc_Stopping);
    mp_event_ctx_wait_state(&ctx, libvlc_Stopped);

    vlc_sem_wait(&imem->done);
    libvlc_media_release(media);

    mp_event_ctx_destroy(&ctx);
    libvlc_media_player_release(player);

    free(imem);
}

static void test_media_callback_interrupt(libvlc_instance_t *vlc)
{
    struct imem_root *imem = imem_root_New();

    static const struct libvlc_media_player_cbs player_cbs = {
        .on_media_stopping = UnblockRead,
    };

    fprintf(stderr, "test: 2/ checking that we can terminate after the input\n");
    libvlc_media_player_t *player =
        libvlc_media_player_new(vlc, LIBVLC_MEDIA_PLAYER_CBS_VER_LATEST,
                                &player_cbs, imem);
    assert(player != NULL);

    static const struct libvlc_media_open_cbs cbs = {
        .open = AccessOpen,
        .read = AccessReadBlocking,
        .close = AccessClose,
    };
    libvlc_media_t *media;

    media = libvlc_media_new_callbacks(LIBVLC_MEDIA_OPEN_CBS_VER_LATEST,
                                       &cbs, imem);
    assert(media != NULL);

    fprintf(stderr, "test: set initial media\n");
    libvlc_media_player_set_media(player, media);
    libvlc_media_player_play(player);

    /* We want to be sure that the media has been opened. */
    vlc_sem_wait(&imem->opened);
    vlc_sem_wait(&imem->read_blocking);

    libvlc_media_release(media);

    media = libvlc_media_new_callbacks(LIBVLC_MEDIA_OPEN_CBS_VER_LATEST,
                                       &cbs, imem);
    assert(media != NULL);
    fprintf(stderr, "test: changing to new media\n");
    libvlc_media_player_set_media(player, media);
    fprintf(stderr, "test: waiting for the new media\n");

    /* Semaphore notifying that the first access has been closed. */
    vlc_sem_wait(&imem->done);
    assert(vlc_sem_trywait(&imem->read_blocking) == EAGAIN);

    vlc_sem_wait(&imem->opened);
    vlc_sem_wait(&imem->read_blocking);

    fprintf(stderr, "test: checking that we get the media stopping event\n");
    libvlc_media_player_stop_async(player);

    vlc_sem_wait(&imem->done);

    libvlc_media_release(media);
    libvlc_media_player_release(player);

    free(imem);
}

int main( int argc, char **argv )
{
    (void)argc; (void)argv;
    test_init();

    const char * const vlc_argv[] = {
        "-vvv", "--vout=dummy", "--aout=dummy", "--text-renderer=dummy",
        "--demux="MODULE_STRING,
    };

    libvlc_instance_t *vlc = libvlc_new(ARRAY_SIZE(vlc_argv), vlc_argv);
    assert(vlc != NULL);

    test_media_callback(vlc);
    test_media_callback_interrupt(vlc);

    libvlc_release(vlc);
    return 0;
}

static int DemuxDemux(demux_t *demux)
{
    (void)demux;
    return VLC_SUCCESS;
}

static void DemuxClose(vlc_object_t *obj)
{
    (void)obj;
}

static int DemuxOpen(vlc_object_t *obj)
{
    demux_t *demux = (demux_t *)obj;

    static const struct vlc_stream_operations ops =
    {
        .demux.demux = DemuxDemux,
    };
    demux->ops = &ops;

    return VLC_SUCCESS;
}

vlc_module_begin()
    set_capability("demux", 0)
    set_callbacks(DemuxOpen, DemuxClose)
vlc_module_end()

VLC_EXPORT const vlc_plugin_cb vlc_static_modules[] = {
    VLC_SYMBOL(vlc_entry),
    NULL
};
