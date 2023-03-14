/*****************************************************************************
 * fingerprintdialog.hpp: Fingerprinter Dialog
 *****************************************************************************
 * Copyright (C) 2012 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef FINGERPRINTDIALOG_HPP
#define FINGERPRINTDIALOG_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qt.hpp"

#include <QDialog>
#include <vlc_fingerprinter.h>

namespace Ui {
class FingerprintDialog;
}

class Chromaprint;

class FingerprintDialog : public QDialog
{
    Q_OBJECT

public:
    FingerprintDialog( QWidget *parent, qt_intf_t *p_intf,
                                input_item_t *p_item );
    virtual ~FingerprintDialog();

private:
    Ui::FingerprintDialog *ui;
    Chromaprint *t;
    fingerprint_request_t *p_r;

private slots:
    void handleResults();
    void applyIdentity();

signals:
    void metaApplied( input_item_t * );
};

#endif // FINGERPRINTDIALOG_HPP
