// SPDX-License-Identifier: LGPL-2.1-or-later
/*****************************************************************************
 * CommonAttributesElements.h: Defines the common attributes and elements
 *                             for some Dash elements.
 *****************************************************************************
 * Copyright © 2011 VideoLAN
 *
 * Authors: Hugo Beauzée-Luyssen <hugo@beauzee.fr>
 *****************************************************************************/

#ifndef COMMONATTRIBUTESELEMENTS_H
#define COMMONATTRIBUTESELEMENTS_H

#include "../tools/Properties.hpp"
#include <string>

namespace adaptive
{
    namespace playlist
    {
        class CommonAttributesElements
        {
            public:
                CommonAttributesElements(CommonAttributesElements * = nullptr);
                virtual ~CommonAttributesElements();
                virtual const std::string&      getMimeType() const;
                void                            setMimeType( const std::string &mimeType );
                int                             getWidth() const;
                void                            setWidth( int width );
                int                             getHeight() const;
                void                            setHeight( int height );
                const AspectRatio &             getAspectRatio() const;
                void                            setAspectRatio(const AspectRatio &);
                const Rate &                    getFrameRate() const;
                void                            setFrameRate(const Rate &);
                const Rate &                    getSampleRate() const;
                void                            setSampleRate(const Rate &);

            protected:
                std::string                         mimeType;
                int                                 width;
                int                                 height;
                AspectRatio                         aspectRatio;
                Rate                                frameRate;
                Rate                                sampleRate;

            private:
                CommonAttributesElements           *parentCommonAttributes;
        };
    }
}

#endif // COMMONATTRIBUTESELEMENTS_H
