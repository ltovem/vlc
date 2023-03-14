/*****************************************************************************
 * customwidgets.hpp: Custom widgets
 ****************************************************************************
 * Copyright (C) 2006 the VideoLAN team
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_QT_CUSTOMWIDGETS_HPP_
#define VLC_QT_CUSTOMWIDGETS_HPP_

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QToolButton>
#include <QDial>

#include "animators.hpp"
#include "qt.hpp"

class QWidget;

class QFramelessButton : public QPushButton
{
    Q_OBJECT
public:
    QFramelessButton( QWidget *parent = NULL );
    QSize sizeHint() const override { return iconSize(); }
protected:
    void paintEvent( QPaintEvent * event ) override;
};

class VLCQDial : public QDial
{
    Q_OBJECT
public:
    VLCQDial( QWidget *parent = NULL );
protected:
    void paintEvent( QPaintEvent * event ) override;
};

class QToolButtonExt : public QToolButton
{
    Q_OBJECT
public:
    QToolButtonExt( QWidget *parent = 0, int ms = 0 );
private:
    bool shortClick;
    bool longClick;
private slots:
    void releasedSlot();
    void clickedSlot();
signals:
    void shortClicked();
    void longClicked();
};

class QVLCDebugLevelSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    QVLCDebugLevelSpinBox( QWidget *parent ) : QSpinBox( parent ) { };
protected:
    QString textFromValue( int ) const override;
    /* QVLCDebugLevelSpinBox is read-only */
    int valueFromText( const QString& ) const override { return -1; }
};

/** This spinning icon, to the colors of the VLC cone, will show
 * that there is some background activity running
 **/
class SpinningIcon : public QLabel
{
    Q_OBJECT

public:
    SpinningIcon( QWidget *parent );
    void play( int loops = -1, int fps = 0 )
    {
        animator->setLoopCount( loops );
        if ( fps ) animator->setFps( fps );
        animator->start();
    }
    void stop() { animator->stop(); }
    bool isPlaying() { return animator->state() == PixmapAnimator::Running; }
private:
    PixmapAnimator *animator;
};

class YesNoCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    YesNoCheckBox( QWidget *parent );
};

/* VLC Key/Wheel hotkeys interactions */

class QKeyEvent;
class QWheelEvent;
class QInputEvent;

int qtKeyModifiersToVLC( const QInputEvent& e );
int qtEventToVLCKey( QKeyEvent *e );
int qtWheelEventToVLCKey( const QWheelEvent& e );
QString VLCKeyToString( unsigned val, bool );

#endif
