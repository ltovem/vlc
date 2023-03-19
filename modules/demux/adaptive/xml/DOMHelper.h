// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * DOMHelper.h
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/

#ifndef DOMHELPER_H_
#define DOMHELPER_H_

#include <vector>
#include <string>

#include "Node.h"

namespace adaptive
{
    namespace xml
    {
        class DOMHelper
        {
            public:
                static std::vector<Node *> getElementByTagName      (Node *root, const std::string& name, bool selfContain);
                static std::vector<Node *> getChildElementByTagName (Node *root, const std::string& name);
                static Node*               getFirstChildElementByName( Node *root, const std::string& name );

            private:
                static void getElementsByTagName(Node *root, const std::string& name, std::vector<Node *> *elements, bool selfContain);
        };
    }
}

#endif /* DOMHELPER_H_ */
