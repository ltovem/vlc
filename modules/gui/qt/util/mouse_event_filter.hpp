/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef MOUSEEVENTFILTER_HPP
#define MOUSEEVENTFILTER_HPP

#include <QObject>
#include <QPointF>
#include <QPointer>

class MouseEventFilter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QObject* target READ target WRITE setTarget NOTIFY targetChanged FINAL)
    Q_PROPERTY(bool filterEventsSynthesizedByQt MEMBER m_filterEventsSynthesizedByQt NOTIFY filterEventsSynthesizedByQtChanged FINAL)

public:
    explicit MouseEventFilter(QObject *parent = nullptr);
    ~MouseEventFilter();

    QObject *target() const;
    void setTarget(QObject *newTarget);

signals:
    void targetChanged();
    void filterEventsSynthesizedByQtChanged();

    void mouseButtonDblClick(QPointF localPos, QPointF globalPos, int buttons, int modifiers, int source, int flags);
    void mouseButtonPress(QPointF localPos, QPointF globalPos, int buttons, int modifiers, int source, int flags);
    void mouseButtonRelease(QPointF localPos, QPointF globalPos, int button, int modifiers, int source, int flags);
    void mouseMove(QPointF localPos, QPointF globalPos, int buttons, int modifiers, int source, int flags);

private:
    bool eventFilter(QObject *watched, QEvent *event) override;

    void attach();
    void detach();

private:
    QPointer<QObject> m_target;
    Qt::MouseButtons m_targetItemInitialAcceptedMouseButtons = Qt::NoButton;
    bool m_filterEventsSynthesizedByQt = false;
};

#endif // MOUSEEVENTFILTER_HPP
