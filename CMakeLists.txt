cmake_minimum_required(VERSION 3.10)
project(MeshGenerator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)

include_directories(include)
add_subdirectory(src)