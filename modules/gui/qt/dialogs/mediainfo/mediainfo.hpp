// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * mediainfo.hpp : Information about a stream
 *****************************************************************************
 * Copyright (C) 2006-2007 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 ******************************************************************************/

#ifndef QVLC_MEDIAINFO_DIALOG_H_
#define QVLC_MEDIAINFO_DIALOG_H_ 1

#include "widgets/native/qvlcframe.hpp"
#include "info_panels.hpp"
#include "util/singleton.hpp"

class QTabWidget;

class MediaInfoDialog : public QVLCFrame, public Singleton<MediaInfoDialog>
{
    Q_OBJECT
public:
    MediaInfoDialog( qt_intf_t *,
                     input_item_t * input = NULL );

    enum panel
    {
        META_PANEL = 0,
        EXTRAMETA_PANEL,
        INFO_PANEL,
        INPUTSTATS_PANEL
    };

    void showTab( panel );
    int currentTab();
#if 0
    void setInput( input_item_t * );
#endif

private:
    virtual ~MediaInfoDialog();

    bool isMainInputInfo;

    QTabWidget *infoTabW;

    InputStatsPanel *ISP;
    MetaPanel *MP;
    InfoPanel *IP;
    ExtraMetaPanel *EMP;

    QPushButton *saveMetaButton;
    QLineEdit   *uriLine;

private slots:
    void updateAllTabs( input_item_t * );
    void clearAllTabs();

    void close() override;

    void saveMeta();
    void updateButtons( int i_tab );
    void updateURI( const QString& );

    friend class    Singleton<MediaInfoDialog>;
};

#endif
