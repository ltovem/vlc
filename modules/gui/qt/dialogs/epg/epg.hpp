/*****************************************************************************
 * epg.hpp : EPG Viewer dialog
 ****************************************************************************
 * Copyright © 2010 VideoLAN and AUTHORS
 *
 * Authors:    Jean-Baptiste Kempf <jb@videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef QVLC_EPG_DIALOG_H_
#define QVLC_EPG_DIALOG_H_ 1

#include "widgets/native/qvlcframe.hpp"

#include "util/singleton.hpp"

class QLabel;
class QTextEdit;
class QTimer;
class EPGItem;
class EPGWidget;

class EpgDialog : public QVLCFrame, public Singleton<EpgDialog>
{
    Q_OBJECT
protected:
    virtual void showEvent(QShowEvent * event) override;

private:
    EpgDialog( qt_intf_t * );
    virtual ~EpgDialog();

    EPGWidget *epg;
    QTextEdit *description;
    QLabel *title;
    QTimer *timer;

    friend class    Singleton<EpgDialog>;

private slots:
    void scheduleUpdate();
    void inputChanged();
    void updateInfos();
    void timeout();
    void displayEvent( EPGItem * );
};

#endif

