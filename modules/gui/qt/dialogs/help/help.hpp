// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * help.hpp : Help and About dialogs
 *****************************************************************************
 * Copyright (C) 2007 the VideoLAN team
 *
 * Authors: Jean-Baptiste Kempf <jb (at) videolan.org>
 *****************************************************************************/

#ifndef QVLC_HELP_DIALOG_H_
#define QVLC_HELP_DIALOG_H_ 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt.hpp"

#include "widgets/native/qvlcframe.hpp"
#include "util/singleton.hpp"

/* Auto-generated from .ui files */
#include "ui_about.h"
#include "ui_update.h"

class QEvent;

class HelpDialog : public QVLCFrame, public Singleton<HelpDialog>
{
    Q_OBJECT
private:
    HelpDialog( qt_intf_t * );
    virtual ~HelpDialog();

public slots:
    void close() override { toggleVisible(); }

    friend class    Singleton<HelpDialog>;
};

class AboutDialog : public QVLCDialog, public Singleton<AboutDialog>
{
    Q_OBJECT
private:
    AboutDialog( qt_intf_t * );
    Ui::aboutWidget ui;

public slots:
    friend class    Singleton<AboutDialog>;

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event) override;
    virtual void showEvent ( QShowEvent * ) override;

private:
    bool b_advanced;

private slots:
    void showLicense();
    void showAuthors();
    void showCredit();
};

#if defined(UPDATE_CHECK)

class UpdateDialog : public QVLCFrame, public Singleton<UpdateDialog>
{
    Q_OBJECT
public:
    static const QEvent::Type UDOkEvent;
    static const QEvent::Type UDErrorEvent;
    void updateNotify( bool );

private:
    UpdateDialog( qt_intf_t * );
    virtual ~UpdateDialog();

    Ui::updateWidget ui;
    update_t *p_update;
    void customEvent( QEvent * ) override;
    bool b_checked;

private slots:
    void close() override { toggleVisible(); }

    void UpdateOrDownload();

    friend class    Singleton<UpdateDialog>;
};
#endif

#endif
