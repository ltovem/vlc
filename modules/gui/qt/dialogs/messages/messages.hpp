/*****************************************************************************
 * messages.hpp : Information about a stream
 ****************************************************************************
 * Copyright (C) 2006-2007 the VideoLAN team
 *
 * Authors: Jean-Baptiste Kempf <jb (at) videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef QVLC_MESSAGES_DIALOG_H_
#define QVLC_MESSAGES_DIALOG_H_ 1

#include "widgets/native/qvlcframe.hpp"
#include "util/singleton.hpp"

/* Auto-generated from .ui files */
#include "ui_messages_panel.h"

#include <QMutex>

class QTreeWidget;
class QTreeWidgetItem;
class MsgEvent;

class MessagesDialog : public QVLCFrame, public Singleton<MessagesDialog>
{
    Q_OBJECT
private:
    MessagesDialog( qt_intf_t * );
    virtual ~MessagesDialog();

    Ui::messagesPanelWidget ui;
    static void sinkMessage( void *, vlc_log_t *, unsigned );
    void customEvent( QEvent * );
    void sinkMessage( const MsgEvent * );
    bool matchFilter( const QString& );

    std::atomic<int> verbosity;
    static void MsgCallback( void *, int, const vlc_log_t *, const char *,
                             va_list );

private slots:
    bool save();
    void updateConfig();
    void changeVerbosity( int );
    void updateOrClear();
    void tabChanged( int );
    void filterMessages();
    void addError( int );
    void errorsCountChanged();

private:
    void buildTree( QTreeWidgetItem *, vlc_object_t * );

    friend class    Singleton<MessagesDialog>;
    QToolButton *updateButton;
    QMutex messageLocker;
#ifndef NDEBUG
    QTreeWidget *pldebugTree;
    void updatePLTree();
#endif

public:
    void showTab( int i );
};

#endif
