#!/usr/bin/python

# Script to generate builder_data.hpp, with the definitions given in
# builder_data.def
# Each line of the definition file is in the following format:
# ClassName param1:type1 param2:type2 ...

import string

deffile = open("builder_data.def")
hppfile = open("builder_data.hpp","w")

hppfile.write(
"""// SPDX-License-Identifier: GPL-2.0-or-later
/*****************************************************************************
 * builder_data.hpp
 *****************************************************************************
 * Copyright (C) 2003 the VideoLAN team
 * $Id$
 *
 * Authors: Cyril Deguet     <asmax@via.ecp.fr>
 *          Olivier Teuliere <ipkiss@via.ecp.fr>
 *****************************************************************************/

//File generated by gen_builder.py
//DO NOT EDIT BY HAND !

#ifndef BUILDER_DATA_HPP
#define BUILDER_DATA_HPP

#include <vlc_common.h>
#include <list>
#include <map>
#include <string>

using namespace std;

/// Structure for mapping data from XML file
struct BuilderData
{

""")

while 1:
    line = string.strip(deffile.readline())
    if line == "":
        break
    items = string.split(line, ' ')
    name = items[0]
    str = "    /// Type definition\n"
    str += "    struct " + name + "\n    {\n"
    str += "        " + name + "( "
    constructor = ""
    initlist = ""
    vars = ""
    for var in items[1:]:
        vardef = string.split(var, ':');
        varname = vardef[0]
        vartype = vardef[1]
        if vartype == "string":
            vartype = "const string &"
        if constructor != "":
            constructor += ", "
        constructor += vartype + " " + varname
        if initlist != "":
            initlist += ", "
        initlist += "m_" + varname + "( " + varname + " )"
        vartype = vardef[1]
        vars += "        " + vartype + " m_" + varname + ";\n"
    str += constructor + " ):\n" + initlist + " {}\n\n"
    str += vars + "    };\n"
    str += "    /// List\n"
    str += "    list<" + name + "> m_list" + name + ";\n"
    str += "\n"
    hppfile.write(str)

hppfile.write(
"""
};

#endif
""")

deffile.close()
hppfile.close()
