/*****************************************************************************
 * preferences.cpp : Preferences
 *****************************************************************************
 * Copyright (C) 2006-2007 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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

#include "preferences.hpp"
#include "expert_view.hpp"
#include "complete_preferences.hpp"
#include "dialogs/errors/errors.hpp"
#include "widgets/native/searchlineedit.hpp"
#include "maininterface/mainctx.hpp"

#include <QPushButton>
#include <QToolButton>
#include <QMessageBox>
#include <QShortcut>

#include <vlc_modules.h>
#include <vlc_config_cat.h>

PrefsDialog::PrefsDialog( QWindow *parent, qt_intf_t *_p_intf )
            : QVLCDialog( parent, _p_intf )
{
    ui.setupUi( this );

    setWindowRole( "vlc-preferences" );

    /* Whether we want it or not, we need to destroy on close to get
       consistency when reset */
    setAttribute( Qt::WA_DeleteOnClose );

    for( int i = 0; i < SPrefsMax ; i++ )
        simple_panels[i] = nullptr;

    ui.advanced_interface->setSizes( QList<int>() << 300 << 550 );

    QPushButton *save = new QPushButton( qtr( "&Save" ) );
    save->setToolTip( qtr( "Save and close the dialog" ) );
    QPushButton *cancel = new QPushButton( qtr( "&Cancel" ) );
    QPushButton *reset = new QPushButton( qtr( "&Reset Preferences" ) );

    ui.button_box->addButton( save, QDialogButtonBox::AcceptRole );
    ui.button_box->addButton( cancel, QDialogButtonBox::RejectRole );
    ui.button_box->addButton( reset, QDialogButtonBox::ResetRole );

    switch( var_InheritInteger( p_intf, "qt-initial-prefs-view" ) )
    {
        case 2:  setExpert();   break;
        case 1:  setAdvanced(); break;
        default: setSimple();   break;
    }

    BUTTONACT( save, &PrefsDialog::save );
    BUTTONACT( cancel, &PrefsDialog::cancel );
    BUTTONACT( reset, &PrefsDialog::reset );

    BUTTONACT( ui.simple, &PrefsDialog::setSimple );
    BUTTONACT( ui.advanced, &PrefsDialog::setAdvanced );
    BUTTONACT( ui.expert, &PrefsDialog::setExpert );

    search = new QShortcut( QKeySequence( QKeySequence::Find ), this );
    connect( search, &QShortcut::activated, this, &PrefsDialog::setSearchFocus );

    QVLCTools::restoreWidgetPosition( p_intf, "Preferences", this, QSize( 850, 700 ) );
}

PrefsDialog::~PrefsDialog()
{
    module_list_free( p_list );
}

void PrefsDialog::setExpert()
{
    /* Lazy creation */
    if( !expert_initialised )
    {
        if ( !p_list )
            p_list = module_list_get( &count );

        ExpertPrefsTableModel *table_model = new ExpertPrefsTableModel( p_list, count, this );
        ui.expert_table->setModel( table_model );
        ui.expert_table->resizeColumnToContents( ExpertPrefsTableModel::NameField );

        connect( ui.expert_table->selectionModel(), &QItemSelectionModel::currentChanged,
                 this, &PrefsDialog::changeExpertDesc );
        connect( ui.expert_table_filter, &SearchLineEdit::textChanged,
                 this, &PrefsDialog::expertTableFilterChanged );
        connect( ui.expert_table_filter_modified, &QCheckBox::toggled,
                 this, &PrefsDialog::expertTableFilterModifiedToggled );

        /* Set initial selection */
        ui.expert_table->setCurrentIndex(
                ui.expert_table->model()->index( 0, 0, QModelIndex() ) );

        expert_initialised = true;
    }

    ui.expert->setChecked( true );
    ui.interface_stack->setCurrentIndex( EXPERT );
    setWindowTitle( qtr( "Expert Preferences" ) );
}

void PrefsDialog::setAdvanced()
{
    /* Lazy creation */
    if( !advanced_initialised )
    {
        if ( !p_list )
            p_list = module_list_get( &count );

        ui.advanced_tree->init( p_intf, p_list, count );

        connect( ui.advanced_tree, &PrefsTree::currentItemChanged, this, &PrefsDialog::changeAdvPanel );
        connect( ui.advanced_tree_filter, &SearchLineEdit::textChanged,
                 this, &PrefsDialog::advancedTreeFilterChanged );
        connect( ui.advanced_current_filter, &QCheckBox::stateChanged,
                 this, &PrefsDialog::onlyLoadedToggled );

        /* Set initial selection */
        ui.advanced_tree->setCurrentIndex(
                ui.advanced_tree->model()->index( 0, 0, QModelIndex() ) );

        advanced_initialised = true;
    }

    ui.advanced->setChecked( true );
    ui.interface_stack->setCurrentIndex( ADVANCED );
    setWindowTitle( qtr( "Advanced Preferences" ) );
}

void PrefsDialog::setSimple()
{
    if( !simple_initialised )
    {
        QToolButton *button;
        QPixmap scaled;
        qreal dpr = devicePixelRatioF();

        #define SPREFS_ICON_SIZE 48
        #define INIT_SPREFS_BTN( button_id, label, ltooltip, icon )                 \
            button = ui.button_ ## button_id;                                       \
            /* Scale icon to non native size outside of toolbutton to avoid widget size */\
            /* computation using native size */                                     \
            scaled = QPixmap( ":/prefsmenu/" #icon ".png" )                         \
                     .scaledToHeight( SPREFS_ICON_SIZE * dpr, Qt::SmoothTransformation );\
            scaled.setDevicePixelRatio( dpr );                                      \
            button->setIcon( scaled );                                              \
            button->setText( qfut( label ) );                                       \
            button->setToolTip( qfut( ltooltip ) );                                 \
            button->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );              \
            button->setIconSize( QSize( SPREFS_ICON_SIZE, SPREFS_ICON_SIZE ) );     \
            button->setMinimumWidth( 40 + SPREFS_ICON_SIZE );                       \
            connect( button, &QToolButton::clicked, this, [=]() {                   \
                this->changeSimplePanel( (int)button_id );                          \
            } );

        INIT_SPREFS_BTN( SPrefsInterface,      INTF_TITLE,    INTF_TOOLTIP,    cone_interface_64 );
        INIT_SPREFS_BTN( SPrefsAudio,          AUDIO_TITLE,   AUDIO_TOOLTIP,   cone_audio_64 );
        INIT_SPREFS_BTN( SPrefsVideo,          VIDEO_TITLE,   VIDEO_TOOLTIP,   cone_video_64 );
        INIT_SPREFS_BTN( SPrefsSubtitles,      SUBPIC_TITLE,  SUBPIC_TOOLTIP,  cone_subtitles_64 );
        INIT_SPREFS_BTN( SPrefsInputAndCodecs, INPUT_TITLE,   INPUT_TOOLTIP,   cone_input_64 );
        INIT_SPREFS_BTN( SPrefsHotkeys,        HOTKEYS_TITLE, HOTKEYS_TOOLTIP, cone_hotkeys_64 );
        INIT_SPREFS_BTN( SPrefsMediaLibrary,   ML_TITLE,      ML_TOOLTIP,      cone_medialibrary_64 );

        #undef INIT_SPREFS_BTN
        #undef SPREFS_ICON_SIZE

        changeSimplePanel( SPrefsDefaultCat );

        simple_initialised = true;
    }

    ui.simple->setChecked( true );
    ui.interface_stack->setCurrentIndex( SIMPLE );
    setWindowTitle( qtr( "Simple Preferences" ) );
}

/* Switching from on simple panel to another */
void PrefsDialog::changeSimplePanel( int number )
{
    if( ! simple_panels[number] )
    {
        SPrefsPanel *insert = new SPrefsPanel( p_intf, ui.simple_panel_stack, number ) ;
        ui.simple_panel_stack->insertWidget( number, insert );
        simple_panels[number] = insert;
    }
    ui.simple_panel_stack->setCurrentWidget( simple_panels[number] );
}

/* Changing from one Advanced Panel to another */
void PrefsDialog::changeAdvPanel( QTreeWidgetItem *item )
{
    if( item == NULL ) return;
    PrefsTreeItem *node = static_cast<PrefsTreeItem *>( item );

    if( !node->panel )
    {
        node->panel = new AdvPrefsPanel( p_intf, ui.advanced_panel_stack, node );
        ui.advanced_panel_stack->addWidget( node->panel );
    }
    ui.advanced_panel_stack->setCurrentWidget( node->panel );
}

/* Changing from one Expert item description to another */
void PrefsDialog::changeExpertDesc( const QModelIndex &current, const QModelIndex &previous )
{
    Q_UNUSED( previous );
    if( !current.isValid() )
       return;
    ExpertPrefsTableItem *item = ui.expert_table->myModel()->itemAt( current );

    ui.expert_item_text->setText( item->getTitle() );
    ui.expert_item_longtext->setText( item->getDescription() );
}

/* Actual apply and save for the preferences */
void PrefsDialog::save()
{
    if( ui.simple->isChecked() && ui.simple_tabs->isVisible() )
    {
        msg_Dbg( p_intf, "Saving the simple preferences" );
        for( int i = 0 ; i< SPrefsMax; i++ ){
            if( ui.simple_panel_stack->widget(i) )
                qobject_cast<SPrefsPanel *>(ui.simple_panel_stack->widget(i))->apply();
        }
    }
    else if( ui.advanced->isChecked() && ui.advanced_tree->isVisible() )
    {
        msg_Dbg( p_intf, "Saving the advanced preferences" );
        ui.advanced_tree->applyAll();
    }
    else if( ui.expert->isChecked() && ui.expert_table->isVisible() )
    {
        msg_Dbg( p_intf, "Saving the expert preferences" );
        ui.expert_table->applyAll();
    }

    /* Save to file */
    if( config_SaveConfigFile( p_intf ) != 0 )
    {
        ErrorsDialog::getInstance (p_intf)->addError( qtr( "Cannot save Configuration" ),
            qtr("Preferences file could not be saved") );
    }

    if( p_intf->p_mi )
        p_intf->p_mi->reloadPrefs();
    accept();

    QVLCTools::saveWidgetPosition( p_intf, "Preferences", this );

}

/* Clean the preferences, dunno if it does something really */
void PrefsDialog::cancel()
{
    QVLCTools::saveWidgetPosition( p_intf, "Preferences", this );

    reject();
}

/* Reset all the preferences, when you click the button */
void PrefsDialog::reset()
{
    int ret = QMessageBox::question(
                 this,
                 qtr( "Reset Preferences" ),
                 qtr( "Are you sure you want to reset your VLC media player preferences?" ),
                 QMessageBox::Ok | QMessageBox::Cancel,
                 QMessageBox::Ok);

    if( ret == QMessageBox::Ok )
    {
        config_ResetAll();
        config_SaveConfigFile( p_intf );
        getSettings()->clear();
        p_intf->p_mi->reloadPrefs();
        p_intf->p_mi->reloadFromSettings();

#ifdef _WIN32
        simple_panels[0]->cleanLang();
#endif

        accept();
    }
}

void PrefsDialog::expertTableFilterModifiedToggled( bool checked )
{
    ui.expert_table->filter( ui.expert_table_filter->text(), checked );
}

void PrefsDialog::expertTableFilterChanged( const QString & text )
{
    ui.expert_table->filter( text, ui.expert_table_filter_modified->isChecked() );
}

void PrefsDialog::advancedTreeFilterChanged( const QString & text )
{
    ui.advanced_tree->filter( text );
}

void PrefsDialog::onlyLoadedToggled()
{
    ui.advanced_tree->setLoadedOnly( ui.advanced_current_filter->isChecked() );
}

void PrefsDialog::setSearchFocus()
{
    if( ui.advanced->isChecked() )
        ui.advanced_tree_filter->setFocus();
    else if( ui.expert->isChecked() )
        ui.expert_table_filter->setFocus();
}
