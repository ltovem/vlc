/*****************************************************************************
 * plugins.hpp : Qt static plugin integration
 ****************************************************************************
 * Copyright © 2006-2009 the VideoLAN team
 * Copyright © 2022 Videolabs
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *          Jean-Baptiste Kempf <jb@videolan.org>
 *          Alexandre Janniaux <ajanni@videolabs.io>
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *****************************************************************************/

#if !defined(QT_STATIC) && !defined(QT_SHARED)
# error "Make sure qconfig.h was included before"
#endif

#if !defined(Q_IMPORT_PLUGIN)
# error "Make sure QtPlugin was included before"
#endif

#ifdef QT_STATIC /* For static builds */
    Q_IMPORT_PLUGIN(QSvgIconPlugin)
    Q_IMPORT_PLUGIN(QSvgPlugin)
    Q_IMPORT_PLUGIN(QJpegPlugin)
    Q_IMPORT_PLUGIN(QtQuick2Plugin)
    Q_IMPORT_PLUGIN(QtQuickControls2Plugin)
    Q_IMPORT_PLUGIN(QtQuickLayoutsPlugin)
    Q_IMPORT_PLUGIN(QtQuick2WindowPlugin)
    Q_IMPORT_PLUGIN(QtQuickTemplates2Plugin)
    Q_IMPORT_PLUGIN(QtQmlModelsPlugin)
    Q_IMPORT_PLUGIN(QtGraphicalEffectsPlugin)
    Q_IMPORT_PLUGIN(QtGraphicalEffectsPrivatePlugin)
    Q_IMPORT_PLUGIN(QmlShapesPlugin)

    #if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
     Q_IMPORT_PLUGIN(QtQmlPlugin)
    #endif

    #ifdef _WIN32
     Q_IMPORT_PLUGIN(QWindowsVistaStylePlugin)
     Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
    #elif defined(Q_OS_MACOS)
     Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
    #endif

    #if defined(QT5_HAS_X11)
     Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
     Q_IMPORT_PLUGIN(QXcbGlxIntegrationPlugin)
    #endif

    #if defined(QT5_HAS_WAYLAND)
     Q_IMPORT_PLUGIN(QWaylandEglPlatformIntegrationPlugin)
     Q_IMPORT_PLUGIN(QWaylandIntegrationPlugin)
     Q_IMPORT_PLUGIN(QWaylandXdgShellIntegrationPlugin)
    #endif
#endif
