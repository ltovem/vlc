/*****************************************************************************
 * hotkey_editor.cpp : Hotkey editor widget for hotkey editor preferences panel
 ****************************************************************************
 * Copyright (C) 2006-2011 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Antoine Cellerier <dionoea@videolan.org>
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

#include <assert.h>

#include "hotkey_editor.hpp"
#include "widgets/native/customwidgets.hpp"
#include "widgets/native/searchlineedit.hpp"
#include <vlc_modules.h>
#include <vlc_plugin.h>

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QGridLayout>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QAction>
#include <QKeySequence>
#include <QComboBox>
#include <QPushButton>
#include <QMenu>
#include <QGuiApplication>
#include <QClipboard>
#include <QFont>

/**********************************************************************
 * Hotkey editor widget
 **********************************************************************/
HotkeyEditor::HotkeyEditor( QWidget *parent ) : QWidget( parent )
{
    initialised = false;
    table = nullptr;
    search_box = nullptr;
    search_fields = nullptr;
}

void HotkeyEditor::buildAppHotkeysList( QWidget *rootWidget )
{
    QList<QAction *> actionsList = rootWidget->findChildren<QAction *>();
    foreach( const QAction *action, actionsList )
    {
        const QList<QKeySequence> shortcuts = action->shortcuts();
        foreach( const QKeySequence &keySequence, shortcuts )
            existing_keys << keySequence.toString();
    }
}

void HotkeyEditor::init( QTreeWidget *table_, SearchLineEdit *search_box_, QComboBox *search_fields_ )
{
    assert( !initialised );
    initialised = true;

    table = table_;
    search_box = search_box_;
    search_fields = search_fields_;

    search_fields->addItem( qtr("Any field"), ANY_COL );
    search_fields->addItem( qtr("Actions"), ACTION_COL );
    search_fields->addItem( qtr("Hotkeys"), HOTKEY_COL );
    search_fields->addItem( qtr("Global Hotkeys"), GLOBAL_HOTKEY_COL );

    table->setSelectionBehavior( QAbstractItemView::SelectItems );

    /* Get the main Module */
    module_t *p_main = module_get_main();
    assert( p_main );

    /* Access to the module_config_t */
    unsigned confsize;
    module_config_t *p_config;

    p_config = module_config_get (p_main, &confsize);

    QList<module_config_t *> global_keys;
    global_keys.reserve( 112 );
    for (size_t i = 0; i < confsize; i++)
    {
        module_config_t *p_config_item = p_config + i;

        if( p_config_item->i_type != CONFIG_ITEM_KEY )
            continue;

        /* Capture global key items to fill in afterwards */
        if( strncmp( p_config_item->psz_name, "global-", 7 ) == 0 )
        {
            global_keys.append( p_config_item );
            continue;
        }

        KeyTableItem *treeItem = new KeyTableItem();
        treeItem->normal.config_name = p_config_item->psz_name;
        treeItem->normal.default_keys = qfu( p_config_item->orig.psz );
        treeItem->global.config_name = nullptr;
        treeItem->global.default_keys = qfu( "" );

        treeItem->setText( ACTION_COL, qfut( p_config_item->psz_text ) );
        if (p_config_item->psz_longtext)
            treeItem->setToolTip( ACTION_COL, qfut(p_config_item->psz_longtext) );

        treeItem->set_keys( p_config_item->value.psz, HOTKEY_COL );
        treeItem->setToolTip( HOTKEY_COL, qtr("Double click to change.\nDelete key to remove.") );
        treeItem->setToolTip( GLOBAL_HOTKEY_COL, qtr("Double click to change.\nDelete key to remove.") );

        table->addTopLevelItem( treeItem );
    }

    for (int i = 0; i < global_keys.count(); i++)
    {
        for (QTreeWidgetItemIterator iter(table); *iter; ++iter)
        {
            KeyTableItem *item = static_cast<KeyTableItem *>( *iter );

            if( strcmp( item->normal.config_name, global_keys[i]->psz_name + 7 ) == 0 )
            {
                item->global.config_name = global_keys[i]->psz_name;
                item->global.default_keys = qfu( global_keys[i]->orig.psz );
                item->set_keys( global_keys[i]->value.psz, GLOBAL_HOTKEY_COL );
                break;
            }
        }
    }

    module_config_free (p_config);

    table->resizeColumnToContents( ACTION_COL );
    table->resizeColumnToContents( HOTKEY_COL );

//    table->setUniformRowHeights( true );

    table->installEventFilter( this );

    /* Get the app hotkeys from the root widget */
    QWidget *parent_tmp, *rootWidget = this;
    while( (parent_tmp = rootWidget->parentWidget()) != nullptr )
        rootWidget = parent_tmp;
    buildAppHotkeysList( rootWidget );

    connect( table, &QTreeWidget::itemActivated,
             this, QOverload<QTreeWidgetItem *, int>::of(&HotkeyEditor::selectKey) );
    connect( search_box, &SearchLineEdit::textChanged, this, &HotkeyEditor::filter );
    connect( search_fields, QOverload<int>::of(&QComboBox::activated),
             this, &HotkeyEditor::filter );
}

void HotkeyEditor::filter()
{
    const QString &qs_search = search_box->text();
    int i_column = search_fields->itemData( search_fields->currentIndex() ).toInt();
    int i_column_count = 1;
    if( i_column == ANY_COL )
    {
        i_column = 0; // ACTION_COL
        i_column_count = ANY_COL;
    }
    for (QTreeWidgetItemIterator iter(table); *iter; ++iter)
    {
        QTreeWidgetItem *item = *iter;
        bool found = false;
        for( int idx = i_column; idx < (i_column + i_column_count); idx++ )
        {
            if( item->text( idx ).contains( qs_search, Qt::CaseInsensitive ) )
            {
                found = true;
                break;
            }
        }
        item->setHidden( !found );
    }
}

void HotkeyEditor::selectKey( QTreeWidgetItem *item, int column )
{
    selectKey( static_cast<KeyTableItem *>( item ), (enum ColumnIndex) column );
}

void HotkeyEditor::selectKey( KeyTableItem *item, enum ColumnIndex column )
{
    /* This happens when triggered by ClickEater */
    if( item == nullptr )
        item = static_cast<KeyTableItem *>( table->currentItem() );

    /* This can happen when nothing is selected on the treeView
       and the shortcutValue is clicked */
    if( !item ) return;

    /* If clicked on the first column, assuming user wants the normal hotkey */
    if( column == ACTION_COL ) column = HOTKEY_COL;

    /* Launch a small dialog to ask for a new key */
    KeyInputDialog *d = new KeyInputDialog( table, item, column );
    d->setExistingkeysSet( &existing_keys );
    d->exec();

    if( d->result() == QDialog::Accepted )
    {
        /* In case of conflict, reset other keys*/
        if( d->conflicts )
            reassign_key( item, d->vlckey, column );
        else
            item->set_keys( d->vlckey, column );
    }
    else if( d->result() == 2 )
        unset( item, column );

    delete d;
}

void HotkeyEditor::doApply()
{
    for (QTreeWidgetItemIterator iter(table); *iter; ++iter)
    {
        KeyTableItem *item = static_cast<KeyTableItem *>( *iter );

        config_PutPsz( item->normal.config_name, qtu( item->normal.keys ) );
        config_PutPsz( item->global.config_name, qtu( item->global.keys ) );
    }
}

bool HotkeyEditor::eventFilter( QObject *obj, QEvent *e )
{
#ifndef QT_NO_CONTEXTMENU
    if( obj == table && e->type() == QEvent::ContextMenu )
    {
        tableContextMenuEvent( static_cast<QContextMenuEvent*>(e) );
        return true;
    }
#endif

    if( obj != table || e->type() != QEvent::KeyPress )
        return QWidget::eventFilter(obj, e);

    switch( static_cast<QKeyEvent*>(e)->key() )
    {
        case Qt::Key_Escape:
            table->clearFocus();
            return true;

        case Qt::Key_Return:
        case Qt::Key_Enter:
            selectKey( table->currentItem(), table->currentColumn() );
            return true;

        case Qt::Key_Delete:
            if( table->currentColumn() != ACTION_COL )
                unset( table->currentItem(), table->currentColumn() );
            return true;

        default:
            return false;
    }
}

#ifndef QT_NO_CONTEXTMENU
void HotkeyEditor::tableContextMenuEvent( QContextMenuEvent *event )
{
    KeyTableItem *item = static_cast<KeyTableItem *>( this->table->currentItem() );
    if( !item || item->isHidden() )
        return;
    /* Avoid menu from right-click on empty space after last item */
    if( event->reason() == QContextMenuEvent::Mouse &&
        !this->table->itemAt( this->table->viewport()->mapFromGlobal( event->globalPos() ) ) )
        return;

    enum ColumnIndex column = (enum ColumnIndex) this->table->currentColumn();

    bool empty;
    bool matches_default;
    switch ( column )
    {
        case ACTION_COL:
            empty = (item->normal.keys.isEmpty() && item->global.keys.isEmpty());
            matches_default = (item->normal.matches_default && item->normal.matches_default);
            break;
        case HOTKEY_COL:
            empty = item->normal.keys.isEmpty();
            matches_default = item->normal.matches_default;
            break;
        case GLOBAL_HOTKEY_COL:
            empty = item->global.keys.isEmpty();
            matches_default = item->global.matches_default;
            break;
        default:
            unreachable();
    }

    QMenu *menu = new QMenu();
    menu->setAttribute(Qt::WA_DeleteOnClose);

    QAction *modify = new QAction( qtr( "&Modify" ), this->table );
    connect( modify, &QAction::triggered, [=]() { this->selectKey( item, column ); } );
    menu->addAction( modify );

    if( column != ACTION_COL )
    {
        QAction *copy = new QAction( qtr( "&Copy value" ), this->table );
        if( empty )
            copy->setEnabled( false );
        else
            connect( copy, &QAction::triggered, [=]() {
                this->copy_value( item, column );
            } );
        menu->addAction( copy );
    }

    QAction *unset = new QAction( qtr( "&Unset" ), this->table );
    if( empty )
        unset->setEnabled( false );
    else
        connect( unset, &QAction::triggered, [=]() { this->unset( item, column ); } );
    menu->addAction( unset );

    QAction *reset = new QAction( qtr( "&Reset" ), this->table );
    if( matches_default )
        reset->setEnabled( false );
    else
        connect( reset, &QAction::triggered, [=]() {
            if( column != ACTION_COL )
                this->reset( item, column );
            else
            {
                this->reset( item, HOTKEY_COL );
                this->reset( item, GLOBAL_HOTKEY_COL );
            }
        } );
    menu->addAction( reset );

    QString reset_all_label = (column == ACTION_COL) ? qtr( "Reset &all" )
                                                     : qtr( "Reset &all (column)" );
    QAction *reset_all = new QAction( reset_all_label, this->table );
    connect( reset_all, &QAction::triggered, [=]() { this->reset_all( column ); } );
    menu->addAction( reset_all );

    menu->popup( event->globalPos() );
}
#endif // QT_NO_CONTEXTMENU

void HotkeyEditor::unset( QTreeWidgetItem *item, int column )
{
    unset( static_cast<KeyTableItem*>( item ), (enum ColumnIndex) column );
}

void HotkeyEditor::unset( KeyTableItem *item, enum HotkeyEditor::ColumnIndex column )
{
    if( item == nullptr )
        return;
    if( column == ACTION_COL )
        column = HOTKEY_COL;
    item->set_keys( nullptr, column );
}

void HotkeyEditor::reset( KeyTableItem *item, enum HotkeyEditor::ColumnIndex column )
{
    QString item_default = item->get_default_keys( column );
    KeyTableItem *conflict = find_conflict( table, item_default, item, column );
    if( conflict != nullptr )
    {
        KeyConflictDialog *dialog = new KeyConflictDialog( this->table, conflict, column );
        dialog->exec();
        if( dialog->result() == QDialog::Accepted )
            reassign_key( item, item_default, column );
        delete dialog;
    }
    else
        item->set_keys( item_default, column );
}

void HotkeyEditor::reset_all( enum HotkeyEditor::ColumnIndex column )
{
    for (QTreeWidgetItemIterator iter(table); *iter; ++iter)
    {
        KeyTableItem *item = static_cast<KeyTableItem *>( *iter );
        if( column != GLOBAL_HOTKEY_COL )
            item->set_keys( item->get_default_keys( HOTKEY_COL ), HOTKEY_COL );
        if( column != HOTKEY_COL )
            item->set_keys( item->get_default_keys( GLOBAL_HOTKEY_COL ), GLOBAL_HOTKEY_COL );
    }
}

void HotkeyEditor::reassign_key( KeyTableItem *item, QString key,
                                 enum HotkeyEditor::ColumnIndex column )
{
    for (QTreeWidgetItemIterator iter(table); *iter; ++iter)
    {
        KeyTableItem *iter_item = static_cast<KeyTableItem *>( *iter );
        if( iter_item != item )
            iter_item->remove_key( key, column );
    }
    item->set_keys( key, column );
}

void HotkeyEditor::copy_value( KeyTableItem *item, enum HotkeyEditor::ColumnIndex column )
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText( item->get_keys( column ) );
}

KeyTableItem * HotkeyEditor::find_conflict( QTreeWidget *table, QString key,
                                            KeyTableItem *ignore_item,
                                            enum HotkeyEditor::ColumnIndex column )
{
    if( key.isEmpty() )
        return nullptr;
    for (QTreeWidgetItemIterator iter(table); *iter; ++iter)
    {
        KeyTableItem *item = static_cast<KeyTableItem *>( *iter );

        if( item == ignore_item )
            continue;

        if( item->contains_key( key, column ) )
            return item;
    }
    return nullptr;
}

const QString &KeyTableItem::get_keys( enum HotkeyEditor::ColumnIndex column )
{
    if( column == HotkeyEditor::GLOBAL_HOTKEY_COL )
        return global.keys;
    return normal.keys;
}

QString KeyTableItem::get_default_keys( enum HotkeyEditor::ColumnIndex column )
{
    if( column == HotkeyEditor::GLOBAL_HOTKEY_COL )
        return global.default_keys;
    return normal.default_keys;
}

void KeyTableItem::set_keys( QString keys, enum HotkeyEditor::ColumnIndex column )
{
    bool matches_default;
    if( column == HotkeyEditor::GLOBAL_HOTKEY_COL )
    {
        global.keys = keys;
        matches_default = global.matches_default = ( keys == global.default_keys );
    }
    else
    {
        normal.keys = keys;
        matches_default = normal.matches_default = ( keys ==  normal.default_keys );
    }
    setText( column, keys.replace( "\t", ", " ) );
    QFont font = this->font( HotkeyEditor::ACTION_COL );
    font.setWeight( matches_default ? QFont::Weight::Normal : QFont::Weight::Bold );
    setFont( column,  font );
    matches_default = (normal.matches_default && global.matches_default);
    font.setWeight( matches_default ? QFont::Weight::Normal : QFont::Weight::Bold );
    setFont( HotkeyEditor::ACTION_COL,  font );
}

bool KeyTableItem::contains_key( QString key, enum HotkeyEditor::ColumnIndex column )
{
    return get_keys( column ).split( "\t" ).contains( key );
}

void KeyTableItem::remove_key( QString key, enum HotkeyEditor::ColumnIndex column )
{
    QStringList keys_list = get_keys( column ).split( "\t" );
    if( keys_list.removeAll( key ) )
        set_keys( keys_list.join( "\t" ), column );
}

/**
 * Class KeyInputDialog
 **/
KeyInputDialog::KeyInputDialog( QTreeWidget *table_,
                                KeyTableItem * keyItem_,
                                enum HotkeyEditor::ColumnIndex column_ ) :
                                QDialog( table_ ), table( table_ ),
                                keyItem( keyItem_ ), column( column_ )
{
    setModal( true );
    conflicts = false;
    existing_keys = nullptr;

    bool global = ( column == HotkeyEditor::GLOBAL_HOTKEY_COL );

    setWindowTitle( global ? qtr( "Global Hotkey change" )
                           : qtr( "Hotkey change" ) );
    setWindowRole( "vlc-key-input" );

    QVBoxLayout *vLayout = new QVBoxLayout( this );
    selected = new QLabel( qtr( "Press the new key or combination for <b>%1</b>" )
                           .arg( keyItem->text( HotkeyEditor::ACTION_COL ) ) );
    vLayout->addWidget( selected , Qt::AlignCenter );

    warning = new QLabel;
    warning->hide();
    vLayout->insertWidget( 1, warning );

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    ok = new QPushButton( qtr("Assign") );
    QPushButton *cancel = new QPushButton( qtr("Cancel") );
    unset = new QPushButton( qtr("Unset") );
    buttonBox->addButton( ok, QDialogButtonBox::AcceptRole );
    buttonBox->addButton( unset, QDialogButtonBox::ActionRole );
    buttonBox->addButton( cancel, QDialogButtonBox::RejectRole );
    ok->setDefault( true );

    ok->setFocusPolicy(Qt::NoFocus);
    unset->setFocusPolicy(Qt::NoFocus);
    cancel->setFocusPolicy(Qt::NoFocus);

    vLayout->addWidget( buttonBox );
    ok->hide();

    connect( buttonBox, &QDialogButtonBox::accepted, this, &KeyInputDialog::accept );
    connect( buttonBox, &QDialogButtonBox::rejected, this, &KeyInputDialog::reject );
    BUTTONACT( unset, &KeyInputDialog::unsetAction );
}

void KeyInputDialog::setExistingkeysSet( const QSet<QString> *keyset )
{
    existing_keys = keyset;
}

void KeyInputDialog::checkForConflicts( const QString &sequence )
{
    conflicts = false;
    if ( vlckey == "" )
    {
        accept();
        return;
    }

    KeyTableItem *conflict = HotkeyEditor::find_conflict( table, vlckey, keyItem, column );
    if( conflict != nullptr )
    {
        warning->setText(
                qtr("Warning: this key or combination is already assigned to \"<b>%1</b>\"")
                .arg( conflict->text( HotkeyEditor::ACTION_COL ) ) );
        warning->show();
        ok->show();
        unset->hide();

        conflicts = true;
        return;
    }

    if( existing_keys && !sequence.isEmpty()
        && existing_keys->contains( sequence ) )
    {
        warning->setText(
            qtr( "Warning: <b>%1</b> is already an application menu shortcut" )
                    .arg( sequence )
        );
        warning->show();
        ok->show();
        unset->hide();

        conflicts = true;
    }
    else accept();
}

void KeyInputDialog::keyPressEvent( QKeyEvent *e )
{
    if( e->key() == Qt::Key_Tab ||
        e->key() == Qt::Key_Shift ||
        e->key() == Qt::Key_Control ||
        e->key() == Qt::Key_Meta ||
        e->key() == Qt::Key_Alt ||
        e->key() == Qt::Key_AltGr )
        return;
    int i_vlck = qtEventToVLCKey( e );
    QKeySequence sequence( e->key() | e->modifiers() );
    vlckey = VLCKeyToString( i_vlck, false );
    vlckey_tr = VLCKeyToString( i_vlck, true );
    selected->setText( qtr( "Key or combination: <b>%1</b>" ).arg( vlckey_tr ) );
    checkForConflicts( sequence.toString() );
}

void KeyInputDialog::wheelEvent( QWheelEvent *e )
{
    int i_vlck = qtWheelEventToVLCKey( *e );
    vlckey = VLCKeyToString( i_vlck, false );
    vlckey_tr = VLCKeyToString( i_vlck, true );
    selected->setText( qtr( "Key: <b>%1</b>" ).arg( vlckey_tr ) );
    checkForConflicts( QString() );
}

void KeyInputDialog::unsetAction() { done( 2 ); }

KeyConflictDialog::KeyConflictDialog( QTreeWidget *table, KeyTableItem * item,
                                      enum HotkeyEditor::ColumnIndex column ) :
                                      QDialog( table )
{
    setModal( true );

    bool global = ( column == HotkeyEditor::GLOBAL_HOTKEY_COL );

    setWindowTitle( global ? qtr( "Global Hotkey assignment conflict" )
                           : qtr( "Hotkey assignment conflict" ) );
    setWindowRole( "vlc-key-conflict" );

    QVBoxLayout *vLayout = new QVBoxLayout( this );
    QLabel *warning = new QLabel;
    warning->setText(
            qtr("Warning: this key or combination is already assigned to \"<b>%1</b>\"")
            .arg( item->text( HotkeyEditor::ACTION_COL ) ) );
    vLayout->addWidget( warning , Qt::AlignCenter );

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    QPushButton *force = new QPushButton( qtr("Assign") );
    QPushButton *cancel = new QPushButton( qtr("Cancel") );
    buttonBox->addButton( force, QDialogButtonBox::AcceptRole );
    buttonBox->addButton( cancel, QDialogButtonBox::RejectRole );
    force->setDefault( true );

    vLayout->addWidget( buttonBox );

    connect( buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept );
    connect( buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject );
}
