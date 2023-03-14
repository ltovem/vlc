/*****************************************************************************
 * interface_widgets.hpp : Custom widgets for the main interface
 ****************************************************************************
 * Copyright (C) 2006-2008 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Rafaël Carré <funman@videolanorg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_QT_INTERFACE_WIDGETS_HPP_
#define VLC_QT_INTERFACE_WIDGETS_HPP_

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <QLabel>

class QWidget;
class QMouseEvent;
class MetaPanel;

struct qt_intf_t;
struct input_item_t;

class CoverArtLabel : public QLabel
{
    Q_OBJECT
public:
    CoverArtLabel( QWidget *parent, qt_intf_t * );
    void setItem( input_item_t * );
    virtual ~CoverArtLabel();

protected:
    virtual void mouseDoubleClickEvent( QMouseEvent *event ) override;

private:
    qt_intf_t *p_intf;
    input_item_t *p_item;

public slots:
    void showArtUpdate( const QString& );
    void showArtUpdate( input_item_t * );
    void askForUpdate();
    void setArtFromFile();
    void clear();
};

#endif
