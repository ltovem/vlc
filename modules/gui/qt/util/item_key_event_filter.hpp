/*****************************************************************************
 * Copyright (C) 2019 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef ITEMKEYEVENTFILTER_HPP
#define ITEMKEYEVENTFILTER_HPP

#include <QQuickItem>

/**
 * @brief The ItemKeyEventFilter class allows to register an event filter,
 * which forwards the filtered key events to itself. This might be useful to
 * process key press before they are processed (and eventually accepted) by
 * children components
 *
 * this is usable as
 *
 *  EventFilter {
 *      filterEnabled: true
 *      Keys.onPressed: {
 *          //do stuff
 *          event.accepted = true //to block the event
 *      }
 *  }
 *
 *  Component.onCompleted: {
 *      filter.source = rootWindow
 *  }
 *
 */
class ItemKeyEventFilter : public QQuickItem
{
    Q_OBJECT
public:
    Q_PROPERTY(QObject * target MEMBER m_target WRITE setTarget FINAL)
    Q_PROPERTY(bool enabled MEMBER m_enabled FINAL)

public:
    ItemKeyEventFilter(QQuickItem *parent = nullptr);
    ~ItemKeyEventFilter();

    void setTarget(QObject *target);

private:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QObject *m_target = nullptr;
    bool m_enabled = true;
    bool m_qmlAccepted = false;
};

#endif // ITEMKEYEVENTFILTER_HPP
