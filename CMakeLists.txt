cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_STATIC_LIBS "Build static libraries" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_APP "Build application" ON)

include(GNUInstallDirs)

add_subdirectory(src)

if(BUILD_APP)
    add_subdirectory(app)
endif()