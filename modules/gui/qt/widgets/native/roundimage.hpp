/*****************************************************************************
 * roundimage.hpp: Custom widgets
 ****************************************************************************
 * Copyright (C) 2021 the VideoLAN team
 *
 * Authors: Prince Gupta <guptaprince8832@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef VLC_QT_ROUNDIMAGE_HPP
#define VLC_QT_ROUNDIMAGE_HPP

#include "qt.hpp"

#include <QImage>
#include <QQuickItem>
#include <QUrl>

class QQuickImageResponse;

class RoundImage : public QQuickItem
{
    Q_OBJECT

    // url of the image
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)

    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)

    Q_PROPERTY(Status status READ status NOTIFY statusChanged FINAL)

public:
    enum Status
    {
        Null,
        Ready,
        Loading,
        Error
    };

    Q_ENUM(Status)

    RoundImage(QQuickItem *parent = nullptr);
    ~RoundImage();

    void componentComplete() override;

    QUrl source() const;
    qreal radius() const;
    Status status() const;

public slots:
    void setSource(const QUrl& source);
    void setRadius(qreal radius);

signals:
    void sourceChanged(const QUrl&);
    void radiusChanged(qreal);
    void statusChanged();

protected:
    void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &value) override;
    QSGNode* updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *) override;

private:
    void setDPR(qreal value);
    void handleImageResponseFinished();
    void resetImageResponse(bool cancel);
    void load();
    void setRoundImage(QImage image);
    void setStatus(const Status status);
    void regenerateRoundImage();

private slots:
    void adjustQSGCustomGeometry(const QQuickWindow* const window);

private:
    QUrl m_source;
    qreal m_radius = 0.0;
    qreal m_dpr = 1.0; // device pixel ratio
    Status m_status = Status::Null;

    bool m_QSGCustomGeometry = false;

    QImage m_roundImage;
    bool m_dirty = false;

    QQuickImageResponse *m_activeImageResponse {};

    bool m_enqueuedGeneration = false;
};

#endif

