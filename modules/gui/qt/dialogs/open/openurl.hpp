// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * openurl.hpp: Open a MRL or clipboard content
 *****************************************************************************
 * Copyright © 2009 the VideoLAN team
 *
 * Authors: Jean-Philippe André <jpeg@videolan.org>
 ******************************************************************************/

#ifndef QVLC_OPEN_URL_H_
#define QVLC_OPEN_URL_H_ 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include "widgets/native/qvlcframe.hpp"
#include "util/singleton.hpp"

class QLineEdit;

class OpenUrlDialog : public QVLCDialog
{
    Q_OBJECT

private:
    QString lastUrl;
    bool bClipboard, bShouldEnqueue;
    QLineEdit *edit;

private slots:
    void enqueue();
    void play();

public:
    OpenUrlDialog( qt_intf_t *, bool bClipboard = true );
    virtual ~OpenUrlDialog() {}

    QString url() const;
    bool shouldEnqueue() const;
    virtual void showEvent( QShowEvent *ev ) override;

public slots:
    void close() override { play(); }

};

#endif
