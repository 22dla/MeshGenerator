cmake_minimum_required(VERSION 3.10)
project(MeshGenerator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/install)

include_directories(include)
add_subdirectory(src)

file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)

file(GLOB files_to_copy ${CMAKE_CURRENT_SOURCE_DIR}/data/*.xyz)

foreach(file ${files_to_copy})
    file(COPY ${file} DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
endforeach()
