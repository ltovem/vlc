// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * podcast_configuration.hpp: Podcast configuration dialog
 *****************************************************************************
 * Copyright (C) 2007 the VideoLAN team
 *
 * Authors: Antoine Cellerier <dionoea at videolan dot org>
 *****************************************************************************/

#ifndef QVLC_PODCAST_CONFIGURATION_DIALOG_H_
#define QVLC_PODCAST_CONFIGURATION_DIALOG_H_ 1

#include "widgets/native/qvlcframe.hpp"
#include "util/singleton.hpp"

/* Auto-generated from .ui files */
#include "ui_podcast_configuration.h"

class PodcastConfigDialog : public QVLCDialog, public Singleton<PodcastConfigDialog>
{
    Q_OBJECT

private:
    PodcastConfigDialog( qt_intf_t * );
    virtual ~PodcastConfigDialog();

    Ui::PodcastConfiguration ui;
public slots:
    void accept();
    void add();
    void remove();

    friend class    Singleton<PodcastConfigDialog>;
};

#endif
