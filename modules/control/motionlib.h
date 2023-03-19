// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * motion.h: laptop built-in motion sensors
 *****************************************************************************
 * Copyright (C) 2012 the VideoLAN team
 *
 * Author: Pierre Ynard
 *****************************************************************************/

#ifndef MOTION_H
#define MOTION_H

typedef struct motion_sensors_t motion_sensors_t;

motion_sensors_t *motion_create( vlc_object_t *obj );
void motion_destroy( motion_sensors_t *motion );
int motion_get_angle( motion_sensors_t *motion );

#endif

