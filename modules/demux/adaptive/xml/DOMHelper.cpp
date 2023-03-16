// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 * DOMHelper.cpp
 *****************************************************************************
 Copyright (C) 2010 - 2011 Klagenfurt University

 Created on: Aug 10, 2010
 Authors: Christopher Mueller <christopher.mueller@itec.uni-klu.ac.at>
          Christian Timmerer  <christian.timmerer@itec.uni-klu.ac.at>
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "DOMHelper.h"

using namespace adaptive::xml;

std::vector<Node *> DOMHelper::getElementByTagName      (Node *root, const std::string& name, bool selfContain)
{
    std::vector<Node *> elements;

    for(size_t i = 0; i < root->getSubNodes().size(); i++)
    {
        getElementsByTagName(root->getSubNodes().at(i), name, &elements, selfContain);
    }

    return elements;
}

std::vector<Node *> DOMHelper::getChildElementByTagName (Node *root, const std::string& name)
{
    std::vector<Node *> elements;

    for(size_t i = 0; i < root->getSubNodes().size(); i++)
    {
        if( root->getSubNodes().at(i)->getName() == name )
            elements.push_back(root->getSubNodes().at(i));
    }

    return elements;
}

void                DOMHelper::getElementsByTagName     (Node *root, const std::string& name, std::vector<Node*> *elements, bool selfContain)
{
    if(!selfContain && !root->getName().compare(name))
    {
        elements->push_back(root);
        return;
    }

    if(!root->getName().compare(name))
        elements->push_back(root);

    for(size_t i = 0; i < root->getSubNodes().size(); i++)
    {
        getElementsByTagName(root->getSubNodes().at(i), name, elements, selfContain);
    }
}

Node*           DOMHelper::getFirstChildElementByName( Node *root, const std::string &name )
{
    for(size_t i = 0; i < root->getSubNodes().size(); i++)
    {
        if( root->getSubNodes().at( i )->getName() == name )
            return root->getSubNodes().at( i );
    }
    return nullptr;
}
