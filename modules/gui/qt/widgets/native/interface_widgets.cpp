/*****************************************************************************
 * interface_widgets.cpp : Custom widgets for the main interface
 ****************************************************************************
 * Copyright (C) 2006-2010 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Rafaël Carré <funman@videolanorg>
 *          Ilkka Ollakka <ileoo@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * ( at your option ) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt.hpp"
#include "interface_widgets.hpp"
#include "dialogs/dialogs_provider.hpp"
#include "player/player_controller.hpp"  /* Speed control */
#include "maininterface/mainctx.hpp" /* Interface integration */
#include "dialogs/mediainfo/info_panels.hpp"

#include <QApplication>
#include <QAction>
#include <QUrl>

#include <math.h>
#include <cassert>

#include <vlc_player.h>
#include <vlc_input_item.h>

CoverArtLabel::CoverArtLabel( QWidget *parent, qt_intf_t *_p_i )
    : QLabel( parent ), p_intf( _p_i )
{
    setContextMenuPolicy( Qt::ActionsContextMenu );
    connect( THEMIM, QOverload<QString>::of(&PlayerController::artChanged),
             this, QOverload<const QString&>::of(&CoverArtLabel::showArtUpdate) );

    setMinimumHeight( 128 );
    setMinimumWidth( 128 );
    setScaledContents( false );
    setAlignment( Qt::AlignCenter );

    QAction *action = new QAction( qtr( "Download cover art" ), this );
    connect( action, &QAction::triggered, this, &CoverArtLabel::requestArtUpdate );
    addAction( action );

    action = new QAction( qtr( "Add cover art from file" ), this );
    connect( action, &QAction::triggered, this, &CoverArtLabel::setArtFromFile );
    addAction( action );

    p_item = SharedInputItem{ THEMIM->getInput() } ;
    if( p_item )
    {
        showArtUpdate( p_item.get() );
    }
    else
        showArtUpdate( "" );
}

CoverArtLabel::~CoverArtLabel()
{
    QList< QAction* > artActions = actions();
    foreach( QAction *act, artActions )
        removeAction( act );
}

void CoverArtLabel::setItem( const SharedInputItem& _p_item )
{
    p_item = _p_item;
}

void CoverArtLabel::mouseDoubleClickEvent( QMouseEvent *event )
{
    if( ! p_item && qobject_cast<MetaPanel *>(this->window()) == NULL )
    {
        THEDP->mediaInfoDialog();
    }
    event->accept();
}

void CoverArtLabel::showArtUpdate( const QString& url )
{
    QPixmap pix;
    if( !url.isEmpty() && pix.load( url ) )
    {
        pix = pix.scaled( minimumWidth(), minimumHeight(),
                          Qt::KeepAspectRatioByExpanding,
                          Qt::SmoothTransformation );
    }
    else
    {
        pix = QPixmap( ":/placeholder/noart.png" );
    }
    setPixmap( pix );
}

void CoverArtLabel::showArtUpdate( input_item_t *_p_item )
{
    /* not for me */
    if ( _p_item != p_item.get() )
        return;

    QString url;
    if ( _p_item ) url = THEMIM->decodeArtURL( _p_item );
    showArtUpdate( url );
}

void CoverArtLabel::requestArtUpdate()
{
    input_item_t *item = p_item.get();

    if ( !item )
    {
        /* default to current item */
        vlc_player_locker lock{ p_intf->p_player };
        if ( vlc_player_IsStarted( p_intf->p_player ) )
            item = vlc_player_GetCurrentMedia( p_intf->p_player );
    }

    if ( item )
    {
        static const struct vlc_metadata_cbs input_preparser_cbs {
            // on_preparse_ended
            NULL,
            // on_art_fetch_ended
            [](input_item_t *p_item, bool fetched, void *userdata) {
                const auto data = reinterpret_cast<CoverArtLabel *>(userdata);

                const SharedInputItem sharedInputItem{ p_item };

                PlayerController * const playerController = data->p_intf->p_mainPlayerController;

                QMetaObject::invokeMethod( playerController, [playerController, sharedInputItem, fetched]() {
                        playerController->onArtFetchEnded( sharedInputItem.get(), fetched );
                    }, Qt::QueuedConnection );
            },
            // on_subtree_added
            NULL
        };

        libvlc_MetadataRequest( vlc_object_instance(p_intf),
                                item,
                                META_REQUEST_OPTION_FETCH_LOCAL,
                                &input_preparser_cbs, this, 0, NULL );
    }

}

void CoverArtLabel::setArtFromFile()
{
    if( !p_item )
        return;

    QUrl fileUrl = QFileDialog::getOpenFileUrl( this, qtr( "Choose Cover Art" ),
        p_intf->p_mi->getDialogFilePath(), qtr( "Image Files (*.gif *.jpg *.jpeg *.png)" ) );

    if( fileUrl.isEmpty() )
        return;

    THEMIM->setArt( p_item.get(), fileUrl.toString() );
}

void CoverArtLabel::clear()
{
    showArtUpdate( "" );
}
