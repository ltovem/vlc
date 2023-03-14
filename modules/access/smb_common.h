/*****************************************************************************
 * smb_common.h: common strings used by SMB and DSM modules
 *****************************************************************************
 * Copyright (C) 2016 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#define SMB_USER_TEXT N_("Username")
#define SMB_USER_LONGTEXT N_("Username that will be used for the connection, " \
        "if no username is set in the URL.")
#define SMB_PASS_TEXT N_("Password")
#define SMB_PASS_LONGTEXT N_("Password that will be used for the connection, " \
        "if no username or password are set in URL.")
#define SMB_DOMAIN_TEXT N_("SMB domain")
#define SMB_DOMAIN_LONGTEXT N_("Domain/Workgroup that " \
        "will be used for the connection.")

#define SMB1_LOGIN_DIALOG_TITLE N_( "SMBv1 authentication required" )

#define SMB_LOGIN_DIALOG_TITLE N_( "SMB authentication required" )
#define SMB_LOGIN_DIALOG_TEXT N_( "The computer (%s) you are trying to connect "   \
    "to requires authentication.\nPlease provide a username (ideally a "  \
    "domain name using the format DOMAIN;username) and a password." )
