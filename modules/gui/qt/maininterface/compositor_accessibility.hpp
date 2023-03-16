// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2023 the VideoLAN team
 *****************************************************************************/

#ifndef COMPOSITOR_ACCESSIBLITY_H
#define COMPOSITOR_ACCESSIBLITY_H

#include <QString>

class QObject;
class QAccessibleInterface;
class QQuickWindow;

namespace vlc {

class AccessibleRenderWindow
{
public:
    virtual ~AccessibleRenderWindow()  = default;
    virtual QQuickWindow* getOffscreenWindow() const = 0;
};


#if !defined(QT_NO_ACCESSIBILITY) && defined(QT5_DECLARATIVE_PRIVATE)

QAccessibleInterface* compositionAccessibleFactory(const QString &classname, QObject *object);

#endif

}

#endif /* COMPOSITOR_ACCESSIBLITY_H */
