// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * chromaprint.hpp: Fingerprinter helper class
 *****************************************************************************
 * Copyright (C) 2012 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef CHROMAPRINT_HPP
#define CHROMAPRINT_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <QObject>
#include <QString>
#include <vlc_fingerprinter.h>
#include "qt.hpp"

class Chromaprint : public QObject
{
    Q_OBJECT

public:
    Chromaprint( qt_intf_t *p_intf = NULL );
    virtual ~Chromaprint();
    bool enqueue( input_item_t *p_item );
    static int results_available( vlc_object_t *p_this, const char *,
                                  vlc_value_t, vlc_value_t newval, void *param );
    fingerprint_request_t * fetchResults();
    void apply( fingerprint_request_t *, size_t i_id );
    static bool isSupported( QString uri );

signals:
    void finished();

private:
    void finish() { emit finished(); }
    qt_intf_t *p_intf;
    fingerprinter_thread_t *p_fingerprinter;
};

#endif // CHROMAPRINT_HPP
