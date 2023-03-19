// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *****************************************************************************/
#ifndef FLICKABLE_SCROLL_HANDLER_HPP
#define FLICKABLE_SCROLL_HANDLER_HPP

#include <QObject>
#include <QQmlProperty>
#include <QPointer>
#include <QQuickItem>
#include <QQmlParserStatus>

class FlickableScrollHandler : public QObject, public QQmlParserStatus
{
    Q_OBJECT

    Q_PROPERTY(QObject* parent READ parent NOTIFY initialized FINAL)

    Q_PROPERTY(qreal scaleFactor READ scaleFactor WRITE setScaleFactor NOTIFY scaleFactorChanged FINAL)
    Q_PROPERTY(qreal effectiveScaleFactor READ effectiveScaleFactor NOTIFY effectiveScaleFactorChanged FINAL)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged FINAL)
    Q_PROPERTY(bool fallbackScroll MEMBER m_fallbackScroll NOTIFY fallbackScrollChanged FINAL)
    Q_PROPERTY(bool handleOnlyPixelDelta MEMBER m_handleOnlyPixelDelta NOTIFY handleOnlyPixelDeltaChanged FINAL)

    Q_INTERFACES(QQmlParserStatus)

public:
    explicit FlickableScrollHandler(QObject *parent = nullptr);
    ~FlickableScrollHandler();

    qreal scaleFactor() const;
    qreal effectiveScaleFactor() const;
    bool enabled() const;

    void setScaleFactor(qreal newScaleFactor);
    void setEnabled(bool newEnabled);

    void classBegin() override;
    void componentComplete() override;

signals:
    void initialized();

    void scaleFactorChanged();
    void enabledChanged();
    void effectiveScaleFactorChanged();
    void fallbackScrollChanged();

    void handleOnlyPixelDeltaChanged();

private slots:
    void adjustScrollBarV();
    void adjustScrollBarH();

private:
    void attach();
    void detach();

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QPointer<QQuickItem> m_target = nullptr;
    qreal m_scaleFactor;
    qreal m_effectiveScaleFactor;
    bool m_enabled = true;
    bool m_fallbackScroll = false;

    QQmlProperty m_propertyContentX, m_propertyContentY;
    QQmlProperty m_propertyContentHeight, m_propertyContentWidth;
    QQmlProperty m_propertyHeight, m_propertyWidth;

    struct ScrollBar {
        QQmlProperty scrollBar;
        QQmlProperty stepSize;
        QMetaMethod increaseMethod;
        QMetaMethod decreaseMethod;
        QQuickItem* item = nullptr;
        bool valid() const { return item != nullptr; };
    } m_scrollBarV, m_scrollBarH;

    void adjustScrollBar(ScrollBar& scrollBar);
    bool m_handleOnlyPixelDelta = false;
};

#endif // FLICKABLE_SCROLL_HANDLER_HPP
