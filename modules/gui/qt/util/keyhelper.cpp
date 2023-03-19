// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
#include "keyhelper.hpp"

#include <QQmlEngine>

QmlKeyHelper::QmlKeyHelper(QObject* parent)
    : QObject(parent)
{}

bool QmlKeyHelper::call(bool (*fun)(const QKeyEvent *), const QObject *event)
{
    QKeyEvent fakeEvent(QKeyEvent::KeyPress,
                        event->property("key").toInt(),
                        static_cast<Qt::KeyboardModifiers>(event->property("modifiers").toInt()),
                        event->property("text").toString(),
                        event->property("isAutoRepeat").toBool(),
                        event->property("count").toInt()
                        );

    return fun(&fakeEvent);
}
