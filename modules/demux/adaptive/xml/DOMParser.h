// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * DOMParser.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef DOMPARSER_H_
#define DOMPARSER_H_

#include <vlc_common.h>
#include <vlc_stream.h>

#include "Node.h"

namespace adaptive
{
    namespace xml
    {
        class DOMParser
        {
            public:
                DOMParser           ();
                DOMParser           (stream_t *stream);
                virtual ~DOMParser  ();

                bool                parse       (bool);
                bool                reset       (stream_t *);
                Node*               getRootNode ();
                void                print       ();

            private:
                Node                *root;
                stream_t            *stream;

                xml_reader_t        *vlc_reader;

                Node*   processNode             (bool);
                void    addAttributesToNode     (Node *node);
                void    print                   (Node *node, int offset);
        };
    }
}

#endif /* DOMPARSER_H_ */
