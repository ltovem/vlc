/*****************************************************************************
 * configuration.h management of the modules configuration
 *****************************************************************************
 * Copyright (C) 2007 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifndef LIBVLC_CONFIGURATION_H
# define LIBVLC_CONFIGURATION_H 1

/* Internal configuration prototypes and structures */

struct vlc_plugin_t;

struct vlc_param {
    union {
        _Atomic int64_t i; /**< Current value (if integer or boolean) */
        _Atomic float f; /**< Current value (if floating point) */
        char *_Atomic str; /**< Current value (if character string) */
    } value;

    struct vlc_plugin_t *owner;
    unsigned char shortname; /**< Optional short option name */
    unsigned internal:1; /**< Hidden from preferences and help */
    unsigned unsaved:1; /**< Not stored in persistent configuration */
    unsigned safe:1; /**< Safe for untrusted provisioning (playlists) */
    unsigned obsolete:1; /**< Ignored for backward compatibility */
    struct module_config_t item;
};

/**
 * Looks up a configuration parameter by name.
 *
 * \return the configuration parameter, or NULL if not found
 */
struct vlc_param *vlc_param_Find(const char *name);

int vlc_param_SetString(struct vlc_param *param, const char *value);

int  config_AutoSaveConfigFile( libvlc_int_t * );

void config_Free(struct vlc_param *, size_t);

void config_CmdLineEarlyScan( libvlc_int_t *, int, const char *[] );

int config_LoadCmdLine   ( libvlc_int_t *, int, const char *[], int * );
int config_LoadConfigFile( libvlc_int_t * );
bool config_PrintHelp (libvlc_int_t *);
void config_Lock(void);
void config_Unlock(void);

int config_SortConfig (void);
void config_UnsortConfig (void);

bool config_IsSafe (const char *);

/**
 * Gets the arch-specific installation directory.
 *
 * This function determines the directory containing the architecture-specific
 * installed asset files (such as executable plugins and compiled byte code).
 *
 * @return a heap-allocated string (use free() to release it), or NULL on error
 */
char *config_GetLibDir(void) VLC_USED VLC_MALLOC;

/* The configuration file */
#define CONFIG_FILE                     "vlcrc"

#endif
