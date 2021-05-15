cmake_minimum_required (VERSION 3.5)
project(TinyTerm)

set(INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
