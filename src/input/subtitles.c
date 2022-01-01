/*****************************************************************************
 * subtitles.c : subtitles detection
 *****************************************************************************
 * Copyright (C) 2003-2009 VLC authors and VideoLAN
 *
 * Authors: Derk-Jan Hartman <hartman at videolan.org>
 * This is adapted code from the GPL'ed MPlayer (http://mplayerhq.hu)
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

/**
 *  \file
 *  This file contains functions to detect subtitle files.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <ctype.h> /* isalnum() */
#include <unistd.h>
#include <sys/stat.h>

#include <vlc_common.h>
#include <vlc_fs.h>
#include <vlc_url.h>

#include "input_internal.h"

/**
 * The possible extensions for subtitle files we support
 */
static const char *const sub_exts[] = { SLAVE_SPU_EXTENSIONS, "" };

/**
 * Remove file extension in-place
 */
static void filename_strip_ext_inplace(char *str)
{
    char *tmp = strrchr(str, '.');
    if (tmp)
        *tmp = '\0';
}

/**
 * Trim special characters from a filename
 * 
 * Trims whitespaces and other non-alphanumeric
 * characters from filenames.
 * 
 * \warning This function operates on the passed string
 * without copying. It might return a pointer different to
 * the passed one, in case it trims characters at the beginning.
 * Therefore it is essential that the return value is used where
 * the trimmed version of the string is needed and the returned
 * pointer must not be free()d but rather the original pointer!
 */
VLC_USED static char *filename_trim_inplace(char *str)
{
    char *ret = str;
    unsigned char c;

    // Trim leading non-alnum
    while( (c = *str) != '\0' && !isalnum(c) )
        str++;
    ret = str;

    // Trim inline nonalnum groups
    char *writehead = str;
    bool consecutive = false;
    while( (c = *str) != '\0' )
    {
        if ( !isalnum(c) )
        {
            if ( consecutive )
            {
                str++;
                continue;
            }

            consecutive = true;
            *writehead++ = ' ';
        } else {
            consecutive = false;
            *writehead++ = tolower(c);
        }
        str++;
    }
    *writehead = '\0';

    // Remove trailing space, if any
    if ( consecutive )
        *(writehead - 1) = '\0';

    return ret;
}

static int whiteonly( const char *s )
{
    unsigned char c;

    while( (c = *s) != '\0' )
    {
        if( isalnum( c ) )
            return 0;
        s++;
    }
    return 1;
}

static int slave_strcmp( const void *a, const void *b )
{
    const input_item_slave_t *p_slave0 = *((const input_item_slave_t **) a);
    const input_item_slave_t *p_slave1 = *((const input_item_slave_t **) b);

    if( p_slave0 == NULL || p_slave1 == NULL )
        return 0;

    /* We can compare these uris since they come from the file system */
#ifdef HAVE_STRCOLL
    return strcoll( p_slave0->psz_uri, p_slave1->psz_uri );
#else
    return strcmp( p_slave0->psz_uri, p_slave1->psz_uri );
#endif
}

/*
 * Check if a file should be filtered (skipped) as a non-subtitle file.
 *
 * Returns true if it does not end with a known subtitle extension.
 */
bool subtitles_Filter( const char *file )
{
    const char *ext = strrchr( file, '.' );
    if( !ext )
        return true;
    ext++;

    for( size_t i = 0; sub_exts[i][0] != '\0'; i++ )
        if( strcasecmp( sub_exts[i], ext ) == 0 )
            return false;
    return true;
}


/**
 * Convert a list of paths separated by ',' to a char**
 */
static char **paths_to_list( const char *basedir, char *pathlist )
{
    size_t i, num_subdirs;
    char **subdirs; /* list of subdirectories to look in */
    char *parser = pathlist;

    if( !basedir || !pathlist )
        return NULL;

    for( i = 0, num_subdirs = 1; pathlist[i] != '\0'; i++ )
    {
        if( pathlist[i] == ',' )
            num_subdirs++;
    }
    i = 0;

    subdirs = calloc( num_subdirs + 1, sizeof(char*) );
    if( !subdirs )
        return NULL;

    for( i = 0; parser && *parser != '\0' ; )
    {
        while( *parser == ' ' )
            parser++;
        char *subdir = parser;
        char *subdir_end = subdir;
        char ch;
        while( ( ch = *parser ) != '\0' )
        {
            if( ch != ' ' && ch != ',' )
            {
                subdir_end = parser;
                subdir_end++;
            }
            parser++;
            if( ch == ',' )
                break;
        }
        *subdir_end = '\0';
        if( *subdir == '\0' )
            continue;

        bool rel = ( subdir[0] == '.' );
        if( rel )
        {
            if( subdir[1] == DIR_SEP_CHAR )
                subdir += 2;
            else if( subdir[1] == '\0' )
                subdir += 1;
        }
        bool has_sep = ( subdir_end == subdir || *--subdir_end == DIR_SEP_CHAR );

        if( asprintf( &subdirs[i], "%s%s%c", rel ? basedir : "",
                  subdir, has_sep ? '\0' : DIR_SEP_CHAR ) == -1 )
            break;
        i++;
    }
    if( i == 0 )
    {
        free( subdirs );
        return NULL;
    }
    assert(i <= num_subdirs);
    subdirs[i] = NULL;

    return subdirs;
}

/**
 * Search a given directory for subtitle files.
 *
 * \param this the calling \ref input_thread_t
 * \param search_dir the subdirectory to search, or NULL to search the video directory itself
 * \param fuzzy indication of how strict the filename match must be
 * \param video_path the full filepath of the video
 * \param video_dir the directory of the video
 * \param video_file the filename of the video, excluding extension
 * \param slaves_ref list to append items to
 * \param slaves_count_ref length of list
 */
static void search_directory( input_thread_t *this, const char *search_dir,
                              int fuzzy, const char* video_path,
                              const char *video_dir, const char *video_file,
                              input_item_slave_t ***slaves_ref,
                              int *slaves_count_ref )
{
    input_item_slave_t **slaves = *slaves_ref;
    int slaves_count = *slaves_count_ref;

    bool subdir_search = (search_dir != NULL);
    if( !subdir_search )
        search_dir = video_dir;

    DIR *dir_listing = vlc_opendir( search_dir );
    if( dir_listing == NULL )
        return;

    msg_Dbg( this, "looking for a subtitle file in %s", search_dir );

    const char *entry;
    while( (entry = vlc_readdir( dir_listing ) ) )
    {
        if( entry[0] == '.' || subtitles_Filter( entry ) )
            continue;

        char *entry_tmp = strdup(entry);
        if (!entry_tmp)
            break;

        /* retrieve various parts of the filename */
        filename_strip_ext_inplace(entry_tmp);
        const char *entry_tmp_trim = filename_trim_inplace(entry_tmp);

        const char *tmp;
        int priority = 0;
        if( strcmp( entry_tmp_trim, video_file ) == 0 )
        {
            /* matches the movie name exactly */
            priority = SLAVE_PRIORITY_MATCH_ALL;
        }
        else if( (tmp = strstr( entry_tmp_trim, video_file ) ) != NULL )
        {
            /* contains the movie name */
            tmp += strlen( video_file );
            if( whiteonly( tmp ) )
            {
                /* no chars after the movie name */
                priority = SLAVE_PRIORITY_MATCH_RIGHT;
            }
            else
            {
                /* chars after (and possibly in front of) the movie name */
                priority = SLAVE_PRIORITY_MATCH_LEFT;
            }
        }
        else if( !subdir_search )
        {
            /* doesn't contain the movie name, prefer files in video_dir over subdirs */
            priority = SLAVE_PRIORITY_MATCH_NONE;
        }
        free(entry_tmp);
        entry_tmp_trim = NULL;

        if( priority >= fuzzy )
        {
            char *path;
            if( asprintf( &path, "%s%s", search_dir, entry ) < 0 )
                continue;

            struct stat st;
            if( strcmp( path, video_path ) != 0
             && vlc_stat( path, &st ) == 0
             && S_ISREG( st.st_mode ) )
            {
                msg_Dbg( this, "autodetected subtitle: %s with priority %d",
                         path, priority );
                char *uri = vlc_path2uri( path, NULL );
                input_item_slave_t *sub = (uri == NULL) ? NULL :
                    input_item_slave_New( uri, SLAVE_TYPE_SPU, priority );
                if( sub != NULL )
                {
                    sub->b_forced = true;
                    TAB_APPEND(slaves_count, slaves, sub);
                }
                free( uri );
            }
            free( path );
        }
    }
    closedir( dir_listing );

    *slaves_ref = slaves; /* in case of realloc */
    *slaves_count_ref = slaves_count;
}

/**
 * Detect subtitle files.
 *
 * When called this function will split up the psz_name_org string into a
 * directory, filename and extension. It then opens the directory
 * in which the file resides and tries to find possible matches of
 * subtitles files.
 *
 * \ingroup Demux
 * \param p_this the calling \ref input_thread_t
 * \param psz_path a list of subdirectories (separated by a ',') to look in.
 * \param psz_name_org the complete filename to base the search on.
 * \param ppp_slaves an initialized input item slave list to append detected subtitles to
 * \param pi_slaves pointer to the size of the slave list
 * \return VLC_SUCCESS if ok
 */
int subtitles_Detect( input_thread_t *p_this, char *psz_path, const char *psz_name_org,
                      input_item_slave_t ***ppp_slaves, int *pi_slaves )
{
    input_item_slave_t **pp_slaves = *ppp_slaves;
    int i_slaves = *pi_slaves;

    if( !psz_name_org )
        return VLC_EGENERIC;

    int i_fuzzy = var_GetInteger( p_this, "sub-autodetect-fuzzy" );
    if ( i_fuzzy == 0 )
        return VLC_EGENERIC;

    char *psz_fname = vlc_uri2path( psz_name_org );
    if( !psz_fname )
        return VLC_EGENERIC;

    /* extract filename & directory from psz_fname */
    char *f_dir = strdup( psz_fname );
    if( f_dir == NULL )
    {
        free( psz_fname );
        return VLC_ENOMEM;
    }
    char *f_fname_trim = strrchr( psz_fname, DIR_SEP_CHAR );
    if( !f_fname_trim )
    {
        free( f_dir );
        free( psz_fname );
        return VLC_EGENERIC;
    }
    f_fname_trim++; /* Skip the '/' */
    f_dir[f_fname_trim - psz_fname] = '\0'; /* keep dir separator in f_dir */

    filename_strip_ext_inplace(f_fname_trim);
    f_fname_trim = filename_trim_inplace(f_fname_trim);

    /* Split the list of additional subdirectories to look in */
    char **subdirs = paths_to_list( f_dir, psz_path );

    /* Search the same directory */
    search_directory(p_this, NULL, i_fuzzy, psz_fname,
                     f_dir, f_fname_trim, ppp_slaves, pi_slaves);

    /* Search subdirectories */
    if( subdirs != NULL )
    {
        for( size_t j = 0; subdirs[j] != NULL; j++ )
        {
            if( strcmp( subdirs[j], f_dir ) == 0 )
                continue;
            search_directory(p_this, subdirs[j], i_fuzzy, psz_fname,
                             f_dir, f_fname_trim, ppp_slaves, pi_slaves);
            free( subdirs[j] );
        }
        free( subdirs );
    }

    free( f_dir );
    free( psz_fname );

    /* Reject some in special cases */
    for( int i = 0; i < i_slaves; i++ )
    {
        input_item_slave_t *p_sub = pp_slaves[i];

        bool b_reject = false;
        char *psz_ext = strrchr( p_sub->psz_uri, '.' );
        if( !psz_ext )
            continue;
        psz_ext++;

        if( !strcasecmp( psz_ext, "sub" ) )
        {
            for( int j = 0; j < i_slaves; j++ )
            {
                input_item_slave_t *p_sub_inner = pp_slaves[j];

                /* A slave can be null if it's already rejected */
                if( p_sub_inner == NULL )
                    continue;

                /* check that the filenames without extension match */
                if( strncasecmp( p_sub->psz_uri, p_sub_inner->psz_uri,
                    strlen( p_sub->psz_uri ) - 3 ) )
                    continue;

                char *psz_ext_inner = strrchr( p_sub_inner->psz_uri, '.' );
                if( !psz_ext_inner )
                    continue;
                psz_ext_inner++;

                /* check that we have an idx file */
                if( !strcasecmp( psz_ext_inner, "idx" ) )
                {
                    b_reject = true;
                    break;
                }
            }
        }
        else if( !strcasecmp( psz_ext, "cdg" ) )
        {
            if( p_sub->i_priority < SLAVE_PRIORITY_MATCH_ALL )
                b_reject = true;
        }
        if( b_reject )
        {
            pp_slaves[i] = NULL;
            input_item_slave_Delete( p_sub );
        }
    }

    /* Sort alphabetically */
    if( i_slaves > 0 )
        qsort( pp_slaves, i_slaves, sizeof (input_item_slave_t*), slave_strcmp );

    return VLC_SUCCESS;
}
