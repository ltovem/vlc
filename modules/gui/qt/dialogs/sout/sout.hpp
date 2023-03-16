// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * sout.hpp : Stream output dialog ( old-style, ala WX )
 *****************************************************************************
 * Copyright ( C ) 2006 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *****************************************************************************/

#ifndef QVLC_SOUT_DIALOG_H_
#define QVLC_SOUT_DIALOG_H_ 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h> /* Gettext functions */

/* Auto-generated from .ui files */
#include "ui_sout.h"

#include "widgets/native/qvlcframe.hpp"
#include "util/soutchain.hpp"

#include <QWizard>

class QPushButton;


class SoutDialog : public QWizard
{
    Q_OBJECT
public:
    SoutDialog( QWindow* parent, qt_intf_t *, const QString& chain = "");
    virtual ~SoutDialog(){}

    QString getChain(){ return chain; }

protected:
    virtual void done( int );
private:
    Ui::Sout ui;

    QString chain;
    QPushButton *okButton;

    qt_intf_t* p_intf;

public slots:
    void updateChain();

private slots:
    void closeTab( int );
    void addDest();
};

#endif
