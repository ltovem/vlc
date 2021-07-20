# Install script for directory: /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/x86_64-linux-gnu")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/libSPIRV.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/libSPVRemapper.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SPIRV" TYPE FILE FILES
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/bitutils.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/spirv.hpp"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/GLSL.std.450.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/GLSL.ext.EXT.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/GLSL.ext.KHR.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/GlslangToSpv.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/hex_float.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/Logger.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/SpvBuilder.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/spvIR.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/doc.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/disassemble.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/GLSL.ext.AMD.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/GLSL.ext.NV.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/SPVRemapper.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/glslang/SPIRV/doc.h"
    )
endif()

