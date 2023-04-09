file(GLOB SOURCES *cpp)

add_executable(MeshGenerator ${SOURCES})

target_include_directories(MeshGenerator PRIVATE include)

install(TARGETS MeshGenerator DESTINATION bin)