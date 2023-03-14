/*
 * Debug.hpp
 *****************************************************************************
 * Copyright © 2015 VideoLAN and VLC Authors
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *****************************************************************************/
#ifndef DEBUG_HPP
#define DEBUG_HPP

//#define ADAPTATIVE_ADVANCED_DEBUG 0
//#define ADAPTATIVE_BW_DEBUG 0
//#define ADAPTATIVE_SEEK_DEBUG 0
//#define ADAPTIVE_DEBUGGING_LOGIC 0
//#define ADAPTATIVE_CACHE_DEBUG

#ifdef ADAPTATIVE_ADVANCED_DEBUG
  #define AdvDebug(code) code
#else
  #define AdvDebug(code)
#endif

#ifdef ADAPTATIVE_BW_DEBUG
  #define BwDebug(code) code
#else
  #define BwDebug(code)
#endif

#ifdef ADAPTATIVE_SEEK_DEBUG
  #define SeekDebug(code) code
#else
  #define SeekDebug(code)
#endif

#ifdef ADAPTATIVE_CACHE_DEBUG
  #define CacheDebug(code) code
#else
  #define CacheDebug(code)
#endif

#endif // DEBUG_HPP

