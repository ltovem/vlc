// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * vlc_spawn.h: Child process helpers
 *****************************************************************************
 * Copyright © 2020 Rémi Denis-Courmont
 *****************************************************************************/

#ifndef VLC_SPAWN_H
#define VLC_SPAWN_H 1

#include <unistd.h>

/**
 * \defgroup spawn Process management
 * \ingroup os
 * @{
 */

/**
 * Spawn a child process (by file name).
 *
 * This function creates a child process. For all practical purposes, it is
 * identical to vlc_spawnp(), with one exception: vlc_spawn() does <b>not</b>
 * look for the executable file name in the OS-defined search path. Instead the
 * file name must be absolute.
 *
 * \param pid storage space for the child process identifier [OUT]
 * \param path executable file path [IN]
 * \param fdv file descriptor array [IN]
 * \param argv NULL-terminated array of command line arguments [IN]
 *
 * \return 0 on success or a standard error code on failure
 */
VLC_API VLC_USED
int vlc_spawn(pid_t *pid, const char *file, const int *fdv,
              const char *const *argv);

/**
 * Spawn a child process.
 *
 * This function creates a child process. The created process will run with the
 * same environment and privileges as the calling process.
 *
 * An array of file descriptors must be provided for the child process:
 * - fdv[0] is the standard input (or -1 for nul input),
 * - fdv[1] is the standard output (or -1 for nul output),
 * - fdv[2] is the standard error (or -1 to ignore).
 * The array must have at least 4 elements and be terminated by -1.
 * Some platforms will ignore file descriptors other than the three standard
 * ones, so those should not be used in code intended to be portable.
 *
 * vlc_waitpid() must be called to collect outstanding system resources
 * after the child process terminates.
 *
 * \param pid storage space for the child process identifier [OUT]
 * \param path executable path [IN]
 * \param fdv file descriptor array [IN]
 * \param argv NULL-terminated array of command line arguments [IN]
 *
 * \return 0 on success or a standard error code on failure
 */
VLC_API VLC_USED
int vlc_spawnp(pid_t *pid, const char *path, const int *fdv,
              const char *const *argv);

/**
 * Waits for a child process.
 *
 * This function waits until a child process created by vlc_spawn() or
 * vlc_spawnp() has completed and releases any associated system resource.
 *
 * \param pid process identifier as returned by vlc_spawn() or vlc_spawnp()
 *
 * \return If the process terminates cleanly, this function returns the exit
 * code of the process. Otherwise, it returns an implementation-defined value
 * that is not a valid exit code.
 */
VLC_API
int vlc_waitpid(pid_t pid);

/** @} */

#endif
