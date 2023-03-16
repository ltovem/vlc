// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef VIEWBLOCKINGRECTANGLE_HPP
#define VIEWBLOCKINGRECTANGLE_HPP

#include <QQuickItem>
#include <QColor>

class ViewBlockingRectangle : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color MEMBER m_color NOTIFY colorChanged FINAL)

public:
    ViewBlockingRectangle(QQuickItem *parent = nullptr);

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

private:
    QColor m_color;
    bool m_windowChanged = false;

signals:
    void colorChanged();
};

#endif
