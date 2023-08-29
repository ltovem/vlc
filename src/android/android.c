/*****************************************************************************
 * android.c: Android OS-specific initialization
 *****************************************************************************
 * Copyright © 2016 VLC authors and VideoLAN
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
# include <config.h>
#endif

#include <vlc_common.h>

#include <jni.h>


static JavaVM *s_jvm = NULL;

VLC_EXPORT JavaVM* vlc_android_GetJVM(void);

JavaVM* vlc_android_GetJVM()
{
    return s_jvm;
}

/* This function is called when the dynamic library is loaded via the
 * java.lang.System.loadLibrary method. */
jint
JNI_OnLoad(JavaVM *vm, void *reserved)
{
    s_jvm = vm;
    return JNI_VERSION_1_2;
}


void
JNI_OnUnload(JavaVM* vm, void* reserved)
{
    (void) reserved;
}


