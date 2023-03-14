/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef EFFECTS_IMAGE_PROVIDER_HPP
#define EFFECTS_IMAGE_PROVIDER_HPP

#include <QObject>
#include <QUrl>
#include <QSize>
#include <QQuickImageProvider>

class EffectsImageProvider : public QObject, public QQuickImageProvider
{
    Q_OBJECT

    static constexpr const char * providerId = "effects";

public:
    enum Effect
    {
        RectDropShadow = 1,
        RoundedRectDropShadow
    };
    Q_ENUM(Effect)

    explicit EffectsImageProvider(QQmlEngine *engine)
        : QQuickImageProvider(QQuickImageProvider::ImageType::Image,
                              QQmlImageProviderBase::ForceAsynchronousImageLoading)
    {
        assert(engine);

        // Engine will take the ownership; no need to set parent in constructor
        engine->addImageProvider(QLatin1String(providerId), this);
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

    Q_INVOKABLE static QUrl url(Effect effect, const QVariantMap& properties);
};

#endif // EFFECTS_IMAGE_PROVIDER_HPP
