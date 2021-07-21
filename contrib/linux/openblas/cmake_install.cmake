# Install script for directory: /home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lib/libopenblas.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openblas" TYPE FILE FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/openblas_config.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openblas" TYPE FILE FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/generated/f77blas.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openblas" TYPE FILE FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/generated/cblas.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openblas" TYPE FILE FILES
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lapack-netlib/LAPACKE/example/lapacke_example_aux.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lapack-netlib/LAPACKE/include/lapack.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lapack-netlib/LAPACKE/include/lapacke.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lapack-netlib/LAPACKE/include/lapacke_config.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lapack-netlib/LAPACKE/include/lapacke_mangling.h"
    "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lapack-netlib/LAPACKE/include/lapacke_utils.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/openblas/openblas" TYPE FILE FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lapacke_mangling.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/openblas.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/OpenBLAS" TYPE FILE FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/OpenBLASConfig.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/OpenBLAS" TYPE FILE RENAME "OpenBLASConfigVersion.cmake" FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/OpenBLASConfigVersion.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/OpenBLAS/OpenBLASTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/OpenBLAS/OpenBLASTargets.cmake"
         "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/CMakeFiles/Export/share/cmake/OpenBLAS/OpenBLASTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/OpenBLAS/OpenBLASTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/cmake/OpenBLAS/OpenBLASTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/OpenBLAS" TYPE FILE FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/CMakeFiles/Export/share/cmake/OpenBLAS/OpenBLASTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cmake/OpenBLAS" TYPE FILE FILES "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/CMakeFiles/Export/share/cmake/OpenBLAS/OpenBLASTargets-relwithdebinfo.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/interface/cmake_install.cmake")
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/driver/level2/cmake_install.cmake")
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/driver/level3/cmake_install.cmake")
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/driver/others/cmake_install.cmake")
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/kernel/cmake_install.cmake")
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lapack/cmake_install.cmake")
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/utest/cmake_install.cmake")
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/test/cmake_install.cmake")
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/ctest/cmake_install.cmake")
  include("/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/lapack-netlib/TESTING/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/davide/Scrivania/GSOC/VlcJB/vlc/contrib/linux/openblas/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
