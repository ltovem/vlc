/*****************************************************************************
 * Copyright (C) 2021 the VideoLAN team
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef IMAGELUMINANCEXTRACTOR_HPP
#define IMAGELUMINANCEXTRACTOR_HPP

#include <QUrl>

#include "util/asynctask.hpp"

class ImageLuminanceExtractor : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged FINAL)
    Q_PROPERTY(int luminance READ luminance NOTIFY luminanceChanged FINAL)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setIsEnabled NOTIFY enabledChanged FINAL)

public:
    enum Status
    {
        FAILED = -1
    };

    ImageLuminanceExtractor(QObject *parent = nullptr);

    QUrl source() const;
    int luminance() const;
    bool isEnabled() const;

public slots:
    void setSource(const QUrl &source);
    void setIsEnabled(bool enabled);

signals:
    void sourceChanged(QUrl source);
    void luminanceChanged(int luminance);
    void enabledChanged(bool enabled);

private:
    class LuminanceCalculator : public AsyncTask<int>
    {
    public:
        LuminanceCalculator(const QUrl &source);

        int execute() override;

    private:
        QUrl m_source;
    };

    void startTask();
    void setLuminance(int luminance);

    QUrl m_source;
    TaskHandle<LuminanceCalculator> m_task;
    int m_luminance = 0;
    bool m_enabled = false;
    bool m_pendingUpdate = false;
};

#endif // IMAGELUMINANCEXTRACTOR_HPP
