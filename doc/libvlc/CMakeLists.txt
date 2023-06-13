CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
cmake_policy(VERSION 3.20)

project("libvlc samples")

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

find_package(LIBVLC REQUIRED)

if(WIN32)

    add_executable(d3d9_player WIN32 d3d9_player.c)
    target_link_libraries(d3d9_player LIBVLC::LIBVLC d3d9)

    add_executable(d3d11_player WIN32 d3d11_player.cpp)
    target_compile_definitions(d3d11_player PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(d3d11_player LIBVLC::LIBVLC d3d11 d3dcompiler uuid)

    add_executable(win_player WIN32 win_player.c)
    target_link_libraries(win_player LIBVLC::LIBVLC)

endif()


find_package(SDL2 CONFIG QUIET)
find_package(GLEW QUIET)

if (SDL2_FOUND AND GLEW_FOUND)
  find_package(OpenGL REQUIRED)

  add_executable(sdl_player
    sdl_opengl_player.cpp
  )

  target_link_libraries(sdl_player
    LIBVLC::LIBVLC
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    GLEW::glew
    OpenGL::GL
  )
endif()
