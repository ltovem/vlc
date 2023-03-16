// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * Copyright (C) 2022 VLC authors and VideoLAN
 *****************************************************************************/

#ifndef QSGROUNDEDRECTANGULARIMAGENODE_HPP
#define QSGROUNDEDRECTANGULARIMAGENODE_HPP

#include <QSGGeometryNode>

#include <memory>
#include <cmath>

class QSGTextureMaterial;
class QSGOpaqueTextureMaterial;
class QSGTexture;

class QSGRoundedRectangularImageNode : public QSGGeometryNode
{
    template<class T>
    static T material_cast(QSGMaterial* const material);

public:
    struct Shape
    {
        QRectF rect;
        qreal radius = 0.0;

        constexpr bool operator ==(const Shape& b) const
        {
            return (rect == b.rect && qFuzzyCompare(radius, b.radius));
        }

        constexpr bool isValid() const
        {
            return (!rect.isEmpty() && std::isgreaterequal(radius, 0.0));
        }
    };

    QSGRoundedRectangularImageNode();

    // For convenience:
    QSGTextureMaterial* material() const;
    QSGOpaqueTextureMaterial* opaqueMaterial() const;

    void setSmooth(const bool smooth);
    void setTexture(const std::shared_ptr<QSGTexture>& texture);

    inline constexpr Shape shape() const
    {
        return m_shape;
    }

    bool setShape(const Shape& shape);

    inline bool rebuildGeometry()
    {
        return rebuildGeometry(m_shape);
    }

    bool rebuildGeometry(const Shape& shape);

    // Constructs a geometry denoting rounded rectangle using QPainterPath
    static QSGGeometry* rebuildGeometry(const Shape& shape,
                                        QSGGeometry* geometry,
                                        const QSGTexture* const atlasTexture = nullptr);

private:
    std::shared_ptr<QSGTexture> m_texture;
    Shape m_shape;
    bool m_smooth = true;
};

#endif // QSGROUNDEDRECTANGULARIMAGENODE_HPP
