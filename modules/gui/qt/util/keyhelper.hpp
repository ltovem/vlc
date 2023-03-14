/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef KEYHELPER_HPP
#define KEYHELPER_HPP

#include <QKeyEvent>

class QJSEngine;
class QQmlEngine;
class QQuickKeyEvent;

//provide basic key matching
class KeyHelper
{
public:
    static bool matchLeft(const QKeyEvent* event)
    {
        return event->key() == Qt::Key_Left
            || event->matches(QKeySequence::MoveToPreviousChar);
    }

    static bool matchRight(const QKeyEvent* event)
    {
        return event->key() == Qt::Key_Right
            || event->matches(QKeySequence::MoveToNextChar);
    }

    static bool matchUp(const QKeyEvent* event)
    {
        return event->key() == Qt::Key_Up
            || event->matches(QKeySequence::MoveToPreviousLine);
    }

    static bool matchDown(const QKeyEvent* event)
    {
        return event->key() == Qt::Key_Down
            || event->matches(QKeySequence::MoveToNextLine);
    }


    static bool matchPageDown(const QKeyEvent* event)
    {
        return event->key() == Qt::Key_PageDown
            || event->matches(QKeySequence::MoveToNextPage);
    }

    static bool matchPageUp(const QKeyEvent* event)
    {
        return event->key() == Qt::Key_PageUp
            || event->matches(QKeySequence::MoveToPreviousPage);
    }

    static bool matchOk( const QKeyEvent* event )
    {
        return event->key() == Qt::Key_Space
            || event->matches(QKeySequence::InsertParagraphSeparator);
    }

    static bool matchSearch( const QKeyEvent* event )
    {
        return event->key() == Qt::Key_Search
            || event->key() == Qt::Key_Slash
            || ( (event->modifiers() == Qt::ControlModifier) && event->key() == Qt::Key_K ) //global search
            || ( (event->modifiers() == Qt::ControlModifier) && event->key() == Qt::Key_F ); //local search
    }

    static bool matchCancel(const QKeyEvent* event)
    {
        return event->key() == Qt::Key_Backspace
            || event->key() == Qt::Key_Back
            || event->key() == Qt::Key_Cancel
            || event->matches(QKeySequence::Back)
            || event->matches(QKeySequence::Cancel);
    }

private:
    KeyHelper() = delete;
};


//expose KeyHelper to QML
class QmlKeyHelper : public QObject
{
    Q_OBJECT
public:
    QmlKeyHelper(QObject* parent = nullptr);

    Q_INVOKABLE bool matchLeft(QObject* event)     { return call(&KeyHelper::matchLeft, event); }
    Q_INVOKABLE bool matchRight(QObject* event)    { return call(&KeyHelper::matchRight, event); }
    Q_INVOKABLE bool matchUp(QObject* event)       { return call(&KeyHelper::matchUp, event); }
    Q_INVOKABLE bool matchDown(QObject* event)     { return call(&KeyHelper::matchDown, event); }
    Q_INVOKABLE bool matchPageDown(QObject* event) { return call(&KeyHelper::matchPageDown, event); }
    Q_INVOKABLE bool matchPageUp(QObject* event)   { return call(&KeyHelper::matchPageUp, event); }
    Q_INVOKABLE bool matchOk( QObject* event )     { return call(&KeyHelper::matchOk, event); }
    Q_INVOKABLE bool matchSearch( QObject* event ) { return call(&KeyHelper::matchSearch, event); }
    Q_INVOKABLE bool matchCancel(QObject* event)   { return call(&KeyHelper::matchCancel, event); }

private:
    //convert arguments and call the right function
    bool call(bool (*fun)(const QKeyEvent*), const QObject *event);
};

#endif // KEYHELPER_HPP
