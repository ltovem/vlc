/*****************************************************************************
 * intf-prefs.h
 *****************************************************************************
 * Copyright (C) 2001-2019 VLC authors and VideoLAN
 *
 * Authors: Felix Paul Kühne <fkuehne at videolan dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#import "main/VLCMain.h"

@interface VLCMain (OldPrefs)

- (void)resetAndReinitializeUserDefaults;
- (void)migrateOldPreferences;

- (void)resetPreferences;

@end
