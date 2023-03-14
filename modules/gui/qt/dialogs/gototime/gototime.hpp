/*****************************************************************************
 * gototime.hpp : GotoTime dialogs
 ****************************************************************************
 * Copyright (C) 2007 the VideoLAN team
 *
 * Authors: Jean-Baptiste Kempf <jb (at) videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef QVLC_GOTOTIME_DIALOG_H_
#define QVLC_GOTOTIME_DIALOG_H_ 1

#include "widgets/native/qvlcframe.hpp"
#include "util/singleton.hpp"

class QTimeEdit;

class GotoTimeDialog : public QVLCDialog, public Singleton<GotoTimeDialog>
{
    Q_OBJECT
private:
    GotoTimeDialog( qt_intf_t * );
    virtual ~GotoTimeDialog();
    QTimeEdit *timeEdit;
private slots:
    void close() override;
    void cancel() override;
    void reset();

    friend class    Singleton<GotoTimeDialog>;
public:
    void toggleVisible();
};

#endif
