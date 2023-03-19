// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * expert_view.hpp : Detailed preferences overview - view
 *****************************************************************************
 * Copyright (C) 2019-2022 VLC authors and VideoLAN
 *
 * Authors: Lyndon Brown <jnqnfe@gmail.com>
 *****************************************************************************/

#ifndef VLC_QT_EXPERT_PREFERENCES_VIEW_HPP_
#define VLC_QT_EXPERT_PREFERENCES_VIEW_HPP_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "expert_model.hpp"

#include <QTreeView>
#include <QDialog>

#include "qt.hpp"

class QVBoxLayout;
class QAction;
class QContextMenuEvent;

class ConfigControl;
class ExpertPrefsEditDialog;

class ExpertPrefsTable : public QTreeView
{
    Q_OBJECT

public:
    ExpertPrefsTable( QWidget *parent = nullptr );
    ExpertPrefsTableModel *myModel()
    {
        return static_cast<ExpertPrefsTableModel *>( model() );
    }
    void applyAll();
    void filter( const QString &text, bool );

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private:
    void modifyItem( const QModelIndex & );
    void toggleItem( const QModelIndex & );
    QAction *reset_action;
    QAction *toggle_action;
    QAction *modify_action;
    QAction *copy_name_action;
    QAction *copy_value_action;
    ExpertPrefsEditDialog *expert_edit;

private slots:
    void resetItem();
    void toggleItem();
    void modifyItem();
    void copyItemName();
    void copyItemValue();
    void doubleClicked( const QModelIndex & );
};

class ExpertPrefsEditDialog : public QDialog
{
    Q_OBJECT
public:
    ExpertPrefsEditDialog( ExpertPrefsTable * );
    void setControl( ConfigControl *, ExpertPrefsTableItem * );

private:
    void clearControl();
    ExpertPrefsTable *table;
    ExpertPrefsTableItem *table_item;
    QVBoxLayout *layout;
    QWidget *control_widget;
    ConfigControl *control;

private slots:
    void accept();
    void reject();
};

#endif
