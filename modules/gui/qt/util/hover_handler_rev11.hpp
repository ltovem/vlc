/*****************************************************************************
 * Copyright (C) 2023 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/
#ifndef HOVERHANDLERREV11_HPP
#define HOVERHANDLERREV11_HPP

#include <QQuickItem>
#include <QQmlParserStatus>

class HoverHandlerRev11 : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

public:
    Q_PROPERTY(QQuickItem * target MEMBER m_target WRITE setTarget FINAL)
    Q_PROPERTY(bool hovered READ isHovered NOTIFY hoveredChanged FINAL)

public:
    HoverHandlerRev11(QObject *parent = nullptr);
    ~HoverHandlerRev11();

    void setTarget(QQuickItem *target);

    inline bool isHovered() const { return m_hovered; }

signals:
    void hoveredChanged();

private:
    bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    void classBegin() override;
    void componentComplete() override;

private:
    QQuickItem *m_target = nullptr;
    bool m_hovered = false;
};

#endif
