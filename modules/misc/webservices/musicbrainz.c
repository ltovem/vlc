/*****************************************************************************
 * musicbrainz.c : Musicbrainz API lookup
 *****************************************************************************
 * Copyright (C) 2019 VideoLabs, VLC authors and VideoLAN
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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string.h>
#include <limits.h>

#include <vlc_common.h>
#include <vlc_messages.h>

#include "json_helper.h"
#include "musicbrainz.h"

typedef struct
{
    struct json_value *root;
    struct json_object json;
} musicbrainz_lookup_t;

static void musicbrainz_lookup_release(musicbrainz_lookup_t *p)
{
    json_free(&p->json);
    free(p);
}

static musicbrainz_lookup_t * musicbrainz_lookup_new(void)
{
    return calloc(1, sizeof(musicbrainz_lookup_t));
}

static musicbrainz_lookup_t * musicbrainz_lookup(vlc_object_t *p_obj, const char *psz_url)
{
    msg_Dbg(p_obj, "Querying MB for %s", psz_url);
    void *p_buffer = json_retrieve_document(p_obj, psz_url);
    if(!p_buffer)
        return NULL;

    musicbrainz_lookup_t *p_lookup = musicbrainz_lookup_new();

    if(p_lookup)
    {
        struct sys_json jsdata;
        jsdata.logger = p_obj->logger;
        jsdata.input = p_buffer;
        jsdata.size = strlen(jsdata.input);
        jsdata.json_read = Read;

        int val = json_parse(&jsdata, &p_lookup->json);        
        if (val) {
            msg_Dbg( p_obj, "error: could not parse json!");
            return false;
        }
    }
    free(p_buffer);
    return p_lookup;
}

static bool musicbrainz_fill_track(struct json_value *tracknode, musicbrainz_track_t *t)
{
    t->psz_title = json_dupstring(tracknode, "title");

    struct json_value node = json_getbyname(&tracknode->object, "artist-credit");
    if (node.type == JSON_ARRAY)
        t->psz_artist = json_dupstring(&node.array.entries[0], "name");

    node = json_getbyname(&tracknode->object, "position");
    if (node.type == JSON_NUMBER)
        t->i_index = node.number;

    return true;
}

static bool musicbrainz_has_cover_in_releasegroup(struct json_value ** const p_nodes,
                                                  size_t i_nodes,
                                                  const char *psz_group_id)
{
    /* FIXME, not sure it does what I want */
    for(size_t i=0; i<i_nodes; i++)
    {
        struct json_value rgnode = json_getbyname(&p_nodes[i]->object, "release-group");
        const char *psz_id = jsongetstring(&rgnode, "id");
        if(!psz_id || strcmp(psz_id, psz_group_id))
            continue;
        
        struct json_value node = json_getbyname(&p_nodes[i]->object, "cover-art-archive");
        
        node = json_getbyname(&node.object, "front");
        if(node.type != JSON_BOOLEAN || !node.boolean)
            continue;

        return true;
    }

    return false;
}

static char *musicbrainz_fill_artists(const struct json_value *arraynode)
{
    char *psz = NULL;
    if(arraynode->type != JSON_ARRAY || arraynode->array.size < 1)
        return psz;

    size_t i_total = 1;
    size_t len = 0;
    for(size_t i=0; i<arraynode->array.size; i++)
    {
        struct json_value name = json_getbyname(&arraynode->array.entries[i].object, "name");
        if(name.type != JSON_STRING)
            continue;
        len = strlen(name.string);
        if(psz == NULL)
        {
            psz = strdup(name.string);
            i_total = len + 1;
        }
        else
        {
            char *p = realloc(psz, i_total + len + 2);
            if(p)
            {
                psz = p;
                psz = strcat(psz, ", ");
                psz = strncat(psz, name.string, len);
                i_total += len + 2;
            }
        }
    }

    return psz;
}

static bool musicbrainz_fill_release(struct json_value *releasenode, musicbrainz_release_t *r)
{
    struct json_value media = json_getbyname(&releasenode->object, "media");
    if(media.type != JSON_ARRAY ||
       media.array.size == 0)
        return false;
    /* we always use first media */
    media = media.array.entries[0];

    struct json_value tracks = json_getbyname(&media.object, "tracks");
    if(tracks.type != JSON_ARRAY ||
       tracks.array.size == 0)
        return false;

    r->p_tracks = calloc(tracks.array.size, sizeof(*r->p_tracks));
    if(!r->p_tracks)
        return false;

    for(size_t i=0; i<tracks.array.size; i++)
    {
        if(musicbrainz_fill_track(&tracks.array.entries[i], &r->p_tracks[r->i_tracks]))
            r->i_tracks++;
    }

    r->psz_title = json_dupstring(releasenode, "title");
    r->psz_id = json_dupstring(releasenode, "id");

    struct json_value rgnode = json_getbyname(&releasenode->object, "release-group");
    r->psz_date = json_dupstring(&rgnode, "first-release-date");
    r->psz_group_id = json_dupstring(&rgnode, "id");
    
    struct json_value node = json_getbyname(&rgnode.object, "artist-credit");
    r->psz_artist = musicbrainz_fill_artists(&node);
    
    node = json_getbyname(&releasenode->object, "release-events");
    if(node.type == JSON_ARRAY && node.array.size)
        r->psz_date = json_dupstring(&node.array.entries[0], "date");

    return true;
}

void musicbrainz_recording_release(musicbrainz_recording_t *mbr)
{
    for(size_t i=0; i<mbr->i_release; i++)
    {
        free(mbr->p_releases[i].psz_id);
        free(mbr->p_releases[i].psz_group_id);
        free(mbr->p_releases[i].psz_artist);
        free(mbr->p_releases[i].psz_title);
        free(mbr->p_releases[i].psz_date);
        free(mbr->p_releases[i].psz_coverart_url);
        for(size_t j=0; j<mbr->p_releases[i].i_tracks; j++)
        {
            free(mbr->p_releases[i].p_tracks[j].psz_title);
            free(mbr->p_releases[i].p_tracks[j].psz_artist);
        }
        free(mbr->p_releases[i].p_tracks);
    }
    free(mbr->p_releases);
    free(mbr);
}

static musicbrainz_recording_t *musicbrainz_lookup_recording_by_apiurl(vlc_object_t *obj,
                                                                       const char *psz_url)
{
    musicbrainz_recording_t *r = calloc(1, sizeof(*r));
    if(!r)
        return NULL;

    musicbrainz_lookup_t *lookup = musicbrainz_lookup(obj, psz_url);
    if(!lookup)
    {
        free(r);
        return NULL;
    }

    struct json_value releases = json_getbyname(&lookup->json, "releases");
    if (releases.type == JSON_ARRAY &&
        releases.array.size)
    {
        r->p_releases = calloc(releases.array.size, sizeof(*r->p_releases));
        if(r->p_releases)
        {
            for(unsigned i=0; i<releases.array.size; i++)
            {
                struct json_value node = releases.array.entries[i];
                musicbrainz_release_t *p_mbrel = &r->p_releases[r->i_release];
                if (node.type != JSON_OBJECT ||
                    !musicbrainz_fill_release(&node, p_mbrel))
                    continue;

                /* Try to find cover from other releases from the same group */
                if(p_mbrel->psz_group_id && !p_mbrel->psz_coverart_url &&
                   musicbrainz_has_cover_in_releasegroup(&releases.array.entries,
                                                         releases.array.size,
                                                         p_mbrel->psz_group_id))
                {
                    char *psz_art = coverartarchive_make_releasegroup_arturl(
                                        COVERARTARCHIVE_DEFAULT_SERVER,
                                        p_mbrel->psz_group_id );
                    if(psz_art)
                        p_mbrel->psz_coverart_url = psz_art;
                }

                r->i_release++;
            }
        }
    }

    musicbrainz_lookup_release(lookup);

    return r;
}

static char *musicbrainz_build_discid_json_url(const char *psz_server,
                                               const char *psz_disc_id,
                                               const char *psz_tail)
{
    char *psz_url;
    if(asprintf(&psz_url,
                "https://%s/ws/2/discid/%s?"
                "fmt=json"
                "&inc=artist-credits+recordings+release-groups"
                "&cdstubs=no"
                "%s%s",
                psz_server ? psz_server : MUSICBRAINZ_DEFAULT_SERVER,
                psz_disc_id,
                psz_tail ? "&" : "",
                psz_tail ? psz_tail : "" ) > -1 )
    {
        return psz_url;
    }
    return NULL;
}

musicbrainz_recording_t *musicbrainz_lookup_recording_by_toc(musicbrainz_config_t *cfg,
                                                             const char *psz_toc)
{
    char *psz_url = musicbrainz_build_discid_json_url(cfg->psz_mb_server, "-", psz_toc);
    if(!psz_url)
        return NULL;

    musicbrainz_recording_t *r = musicbrainz_lookup_recording_by_apiurl(cfg->obj, psz_url);
    free(psz_url);
    return r;
}

musicbrainz_recording_t *musicbrainz_lookup_recording_by_discid(musicbrainz_config_t *cfg,
                                                                const char *psz_disc_id)
{
    char *psz_url = musicbrainz_build_discid_json_url(cfg->psz_mb_server, psz_disc_id, NULL);
    if(!psz_url)
        return NULL;

    musicbrainz_recording_t *r = musicbrainz_lookup_recording_by_apiurl(cfg->obj, psz_url);
    free(psz_url);
    return r;
}

char * coverartarchive_make_releasegroup_arturl(const char *psz_server, const char *psz_group_id)
{
    char *psz_art;
    if(-1 < asprintf(&psz_art, "https://%s/release-group/%s/front",
                     psz_server ? psz_server : COVERARTARCHIVE_DEFAULT_SERVER,
                     psz_group_id))
        return psz_art;
    return NULL;
}

void musicbrainz_release_covert_art(coverartarchive_t *c)
{
    free(c);
}

coverartarchive_t * coverartarchive_lookup_releasegroup(musicbrainz_config_t *cfg, const char *psz_id)
{
    coverartarchive_t *c = calloc(1, sizeof(*c));
    if(!c)
        return NULL;

    char *psz_url;
    if(asprintf(&psz_url, "https://%s/release-group/%s", cfg->psz_coverart_server, psz_id) < 0)
    {
        free(c);
        return NULL;
    }

     musicbrainz_lookup_t *p_lookup = musicbrainz_lookup(cfg->obj, psz_url);
     free(psz_url);

     if(!p_lookup)
     {
         free(c);
         return NULL;
     }

    return c;
}

