/*****************************************************************************
 * hotkey_editor.hpp : Hotkey editor widget for hotkey editor preferences panel
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

#ifndef VLC_QT_PREFERENCES_HOTKEYS_HPP_
#define VLC_QT_PREFERENCES_HOTKEYS_HPP_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt.hpp"

#include <QWidget>
#include <QTreeWidgetItem>
#include <QDialog>
#include <QSet>

class QWidget;
class QLabel;
class QTreeWidget;
class SearchLineEdit;
class QComboBox;
class QPushButton;
class QContextMenuEvent;

class KeyTableItem;

class HotkeyEditor : public QWidget
{
    Q_OBJECT

public:
    HotkeyEditor( QWidget * );
    void init( QTreeWidget *, SearchLineEdit *, QComboBox * );
    void doApply();
    enum ColumnIndex
    {
        ACTION_COL = 0,
        HOTKEY_COL = 1,
        GLOBAL_HOTKEY_COL = 2,
        ANY_COL = 3 // == count()
    };
    static KeyTableItem *find_conflict( QTreeWidget *, QString, KeyTableItem *, enum ColumnIndex );

protected:
    bool eventFilter( QObject *, QEvent * ) Q_DECL_OVERRIDE;
#ifndef QT_NO_CONTEXTMENU
    void tableContextMenuEvent( QContextMenuEvent * );
#endif
    void unset( KeyTableItem *, enum ColumnIndex );
    void unset( QTreeWidgetItem *, int );
    void reset( KeyTableItem *, enum ColumnIndex );
    void reset_all( enum ColumnIndex column );
    /** Reassign key to specified item */
    void reassign_key( KeyTableItem *item, QString keys, enum ColumnIndex column );
    void copy_value( KeyTableItem *, enum ColumnIndex );

private:
    void selectKey( KeyTableItem *, enum ColumnIndex );
    void buildAppHotkeysList( QWidget *rootWidget );
    SearchLineEdit *search_box;
    QComboBox *search_fields;
    QTreeWidget *table;
    QSet<QString> existing_keys;
    bool initialised;

private slots:
    void selectKey( QTreeWidgetItem *, int );
    void filter();
};

struct KeyItemAttr
{
    const char *config_name;
    QString default_keys;
    QString keys;
    bool matches_default;
};

class KeyTableItem : public QTreeWidgetItem
{
public:
    KeyTableItem() {}
    const QString &get_keys( enum HotkeyEditor::ColumnIndex );
    QString get_default_keys( enum HotkeyEditor::ColumnIndex );
    void set_keys( QString, enum HotkeyEditor::ColumnIndex );
    void set_keys( const char *keys, enum HotkeyEditor::ColumnIndex column )
    {
        set_keys( (keys) ? qfut( keys ) : qfu( "" ), column );
    }
    bool contains_key( QString, enum HotkeyEditor::ColumnIndex );
    void remove_key( QString, enum HotkeyEditor::ColumnIndex );
    struct KeyItemAttr normal;
    struct KeyItemAttr global;
};

class KeyInputDialog : public QDialog
{
    Q_OBJECT

public:
    KeyInputDialog( QTreeWidget *, KeyTableItem *, enum HotkeyEditor::ColumnIndex );
    bool conflicts;
    QString vlckey, vlckey_tr;
    void setExistingkeysSet( const QSet<QString> *keyset = NULL );

private:
    QTreeWidget *table;
    QLabel *selected, *warning;
    QPushButton *ok, *unset;
    KeyTableItem *keyItem;
    enum HotkeyEditor::ColumnIndex column;

    void checkForConflicts( const QString &sequence );
    void keyPressEvent( QKeyEvent *);
    void wheelEvent( QWheelEvent *);
    const QSet<QString> *existing_keys;

private slots:
    void unsetAction();
};

class KeyConflictDialog : public QDialog
{
    Q_OBJECT

public:
    KeyConflictDialog( QTreeWidget *, KeyTableItem *, enum HotkeyEditor::ColumnIndex );
};
#endif
