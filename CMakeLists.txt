cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES main.cpp
            perlin.cpp)

set(PROJECT_NAME perlin)
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${SOURCES})

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} )

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS -Wall)