/*****************************************************************************
 * Copyright (C) 2021 VLC authors and VideoLAN
 *
 * Authors: Benjamin Arnaud <bunjee@omega.gg>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#ifndef COVERGENERATOR_HPP
#define COVERGENERATOR_HPP

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

// MediaLibrary includes
#include "medialibrary/mlqmltypes.hpp"

// Util includes
#include "util/asynctask.hpp"

// Qt includes
#include <QPainter>

// Forward declarations
struct vlc_medialibrary_t;
class MLItemId;

class CoverGenerator
{
public: // Enums
    enum Split
    {
        Divide,
        Duplicate
    };

public:
    CoverGenerator();

public: // Interface
    MLItemId getId();

    void setSize(const QSize & size);

    void setCountX(int x);
    void setCountY(int y);

    // NOTE: Do we want to divide or duplicate thumbnails to reach the proper count ?
    void setSplit(Split split);

    // NOTE: Applies SmoothTransformation to thumbnails. Disabled by default.
    void setSmooth(bool enabled);

    // NOTE: You need to specify a radius to enable blur, 8 looks good.
    void setBlur(int radius);

    void setDefaultThumbnail(const QString & fileName);

    int requiredNoOfThumbnails() const;

    QImage execute(QStringList thumbnails) const;

private: // Functions
    QString fileName() const;

    void draw(QPainter & painter, const QStringList & fileNames, int countX, int countY) const;

    void drawImage(QPainter & painter, const QString & fileName, const QRect & rect) const;

    void blur(QImage &image) const;

    QString getPrefix(vlc_ml_parent_type type) const;

private:
    QSize m_size;

    int m_countX;
    int m_countY;

    Split m_split;

    int m_blur;

    QString m_default;
};

#endif // COVERGENERATOR_HPP
