/*****************************************************************************
 * podcast_configuration.cpp: Podcast configuration dialog
 ****************************************************************************
 * Copyright (C) 2007 the VideoLAN team
 *
 * Authors: Antoine Cellerier <dionoea at videolan dot org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "podcast_configuration.hpp"

PodcastConfigDialog::PodcastConfigDialog( qt_intf_t *_p_intf)
                    : QVLCDialog( nullptr, _p_intf )

{
    ui.setupUi( this );

    ui.podcastDelete->setToolTip( qtr( "Deletes the selected item" ) );
    QPushButton *okButton = new QPushButton( qtr( "&Close" ), this );
    QPushButton *cancelButton = new QPushButton( qtr( "&Cancel" ), this );
    ui.okCancel->addButton( okButton, QDialogButtonBox::AcceptRole );
    ui.okCancel->addButton( cancelButton, QDialogButtonBox::RejectRole );

    connect( ui.podcastAdd, &QPushButton::clicked, this, &PodcastConfigDialog::add );
    connect( ui.podcastDelete, &QPushButton::clicked, this, &PodcastConfigDialog::remove );
    connect( okButton, &QPushButton::clicked, this, &PodcastConfigDialog::close );

    char *psz_urls = config_GetPsz( "podcast-urls" );
    if( psz_urls )
    {
        char *psz_url = psz_urls;
        while( 1 )
        {
            char *psz_tok = strchr( psz_url, '|' );
            if( psz_tok ) *psz_tok = '\0';
            ui.podcastList->addItem( psz_url );
            if( psz_tok ) psz_url = psz_tok+1;
            else break;
        }
        free( psz_urls );
    }
}

PodcastConfigDialog::~PodcastConfigDialog()
{
}

void PodcastConfigDialog::accept()
{
    QString urls = "";
    for( int i = 0; i < ui.podcastList->count(); i++ )
    {
        urls +=  ui.podcastList->item(i)->text();
        if( i != ui.podcastList->count()-1 ) urls += "|";
    }
    config_PutPsz( "podcast-urls", qtu( urls ) );

    //FIXME IsServicesDiscoveryLoaded is uninplmented
    //if( playlist_IsServicesDiscoveryLoaded( THEPL, "podcast" ) )
    //{
    //    var_SetString( THEPL, "podcast-urls", qtu( urls ) );
    //    msg_Dbg( p_intf, "You will need to reload the podcast module to take into account deleted podcast urls" );
    //}
}

void PodcastConfigDialog::add()
{
    if( ui.podcastURL->text() != QString( "" ) )
    {
        ui.podcastList->addItem( ui.podcastURL->text() );
        ui.podcastURL->clear();
    }
}

void PodcastConfigDialog::remove()
{
    delete ui.podcastList->currentItem();
}
