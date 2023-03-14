/*****************************************************************************
 * searchlineedit.hpp: Custom widgets
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_QT_SEARCHLINEEDIT_HPP_
#define VLC_QT_SEARCHLINEEDIT_HPP_

#include "qt.hpp"
#include <QLineEdit>

class QFramelessButton;
class SearchLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    SearchLineEdit( QWidget *parent = NULL );

private:
    void resizeEvent ( QResizeEvent * event );
    void focusInEvent( QFocusEvent *event );
    void focusOutEvent( QFocusEvent *event );
    void paintEvent( QPaintEvent *event );
    void setMessageVisible( bool on );
    QFramelessButton   *clearButton;
    bool message;

public slots:
    void clear();

private slots:
    void updateText( const QString& );
    void searchEditingFinished();

signals:
    void searchDelayedChanged( const QString& );
};

#endif

