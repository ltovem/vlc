/* Copyright Rafaël Carré (licence WTFPL) */
/* A video thumbnailer compatible with nautilus */
/* Copyright © 2007-2011 Rafaël Carré <funman@videolanorg> */

/* Works with : libvlc 2.0.0
   gcc -pedantic -Wall -Werror -Wextra `pkg-config --cflags --libs libvlc` -lpthread

  # to register the thumbnailer on gnome 3.x:
  cp thumb.thumbnailer /usr/share/thumbnailers

  # to register the thumbnailer on gnome 2.x:
  list=`grep ^Mime vlc.desktop|cut -d= -f2-|sed -e s/";"/\\\n/g -e s,/,@,g`
  vid=`echo $mimes|grep ^vid`
  for i in $vid
  do
    key=/desktop/gnome/thumbnailers/$i/enable
    gconftool-2 -t boolean -s $key true
    gconftool-2 -t string  -s $key "vlc-thumb -s %s %u %o"
  done
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <vlc/vlc.h>

/* position at which the snapshot is taken */
#define VLC_THUMBNAIL_POSITION (30./100.)

static void usage(const char *name, int ret)
{
    fprintf(stderr, "Usage: %s [-s width] <video> <output.png>\n", name);
    exit(ret);
}

/* extracts options from command line */
static void cmdline(int argc, const char **argv, const char **in,
                    char **out, char **out_with_ext, int *w)
{
    int idx = 1;
    size_t len;

    if (argc != 3 && argc != 5)
        usage(argv[0], argc != 2 || strcmp(argv[1], "-h"));

    *w = 0;

    if (argc == 5) {
        if (strcmp(argv[1], "-s"))
            usage(argv[0], 1);

        idx += 2; /* skip "-s width" */
        *w = atoi(argv[2]);
    }

    *in  = argv[idx++];
    *out = strdup(argv[idx++]);
    if (!*out)
        abort();

    len = strlen(*out);
    if (len >= 4 && !strcmp(*out + len - 4, ".png")) {
        *out_with_ext = *out;
        return;
    }

    /* We need to add .png extension to filename,
     * VLC relies on it to detect output format,
     * and nautilus doesn't give filenames ending in .png */

    *out_with_ext = malloc(len + sizeof ".png");
    if (!*out_with_ext)
        abort();
    strcpy(*out_with_ext, *out);
    strcat(*out_with_ext, ".png");
}

static libvlc_instance_t *create_libvlc(void)
{
    static const char* const args[] = {
        "--intf", "dummy",                  /* no interface                   */
        "--vout", "dummy",                  /* we don't want video (output)   */
        "--no-audio",                       /* we don't want audio (decoding) */
        "--no-video-title-show",            /* nor the filename displayed     */
        "--no-stats",                       /* no stats                       */
        "--no-sub-autodetect-file",         /* we don't want subtitles        */
        "--no-snapshot-preview",            /* no blending in dummy vout      */
#ifndef NDEBUG
        "--verbose=2",                      /* full log                       */
#endif
    };

    return libvlc_new(sizeof args / sizeof *args, args);
}

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  wait_callback;
static bool done;

static void on_picture(void *data, libvlc_thumbnailer_request_t *req,
                       libvlc_picture_t *picture)
{
    libvlc_picture_t** outpicture = data;
    if (picture != NULL)
        libvlc_picture_retain(picture);

    libvlc_thumbnailer_request_destroy(req);

    pthread_mutex_lock(&lock);
    *outpicture = picture;
    done = true;
    pthread_cond_signal(&wait_callback);
    pthread_mutex_unlock(&lock);
}

#define VLC_THUMBNAIL_TIMEOUT   5 /* 5 secs */

static void snapshot(libvlc_instance_t *vlc, libvlc_media_t *m,
                     int width, char *out_with_ext)
{
    libvlc_picture_t* pic = NULL;
    static const struct libvlc_thumbnailer_cbs thumb_cbs = {
        .on_picture = on_picture,
    };
    libvlc_thumbnailer_t *thumbnailer =
        libvlc_thumbnailer_new(vlc, LIBVLC_THUMBNAILER_CBS_VER_LATEST,
                               &thumb_cbs, &pic);
    assert(thumbnailer);
    done = false;
    libvlc_thumbnailer_request_t* req = libvlc_thumbnailer_request_new(m);
    assert(req != NULL);
    libvlc_thumbnailer_request_set_position(req, VLC_THUMBNAIL_POSITION, true);
    libvlc_thumbnailer_request_set_picture_size(req, width, 0, false);
    libvlc_thumbnailer_request_set_picture_type(req, libvlc_picture_Png);
    libvlc_thumbnailer_request_set_timeout(req, VLC_THUMBNAIL_TIMEOUT * 1000);
    int ret = libvlc_thumbnailer_queue(thumbnailer, req);
    if (ret != 0)
    {
        fprintf(stderr, "Failed to request thumbnail\n");
        exit(1);
    }
    pthread_mutex_lock(&lock);
    while (!done)
        pthread_cond_wait(&wait_callback, &lock);
    pthread_mutex_unlock(&lock);
    libvlc_thumbnailer_destroy(thumbnailer);

    if (!pic)
    {
        fprintf(stderr, "Snapshot has not been written (timeout after %d secs!\n",
                VLC_THUMBNAIL_TIMEOUT);
        exit(1);
    }
    int res = libvlc_picture_save(pic, out_with_ext);
    libvlc_picture_release(pic);
    if (res)
    {
        fprintf(stderr, "Failed to save the thumbnail\n");
        exit(res);
    }
}

int main(int argc, const char **argv)
{
    const char *in;
    char *out, *out_with_ext;
    int width;
    libvlc_instance_t *libvlc;
    libvlc_media_t *m;

    /* mandatory to support UTF-8 filenames (provided the locale is well set)*/
    setlocale(LC_ALL, "");

    cmdline(argc, argv, &in, &out, &out_with_ext, &width);

    pthread_cond_init(&wait_callback, NULL);

    /* starts vlc */
    libvlc = create_libvlc();
    assert(libvlc);

    m = libvlc_media_new_path(in);
    assert(m);

    /* takes snapshot */
    snapshot(libvlc, m, width, out_with_ext);

    /* clean up */
    if (out != out_with_ext) {
        rename(out_with_ext, out);
        free(out_with_ext);
    }
    free(out);

    libvlc_media_release(m);
    libvlc_release(libvlc);

    pthread_cond_destroy(&wait_callback);

    return 0;
}
