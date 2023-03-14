/*****************************************************************************
 * vlc-qt-check.cpp: run-time Qt availability test
 ****************************************************************************
 * Copyright © 2018 Rémi Denis-Courmont
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdlib.h>
#include <QApplication>
#include <QTextStream>
#include <QtGlobal>
#include <iostream>

static void messageOutput(QtMsgType type, const QMessageLogContext &,
                          const QString &msg)
{
    if (type == QtFatalMsg)
    {
        std::cerr << msg.toUtf8().constData() << std::endl;
        exit(1);
    }
}

#include <qconfig.h>
#include <QtPlugin>
QT_BEGIN_NAMESPACE
#include "plugins.hpp"
QT_END_NAMESPACE

int main(int argc, char *argv[])
{
    qInstallMessageHandler(messageOutput);
    QApplication app(argc, argv);
}
