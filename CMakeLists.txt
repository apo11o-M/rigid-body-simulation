# Porjects Settings
cmake_minimum_required(VERSION 3.1)
project(rigid-body-simulation)

add_executable(rigid-body-simulation
    src/fps.cpp
    src/util.cpp
    src/main.cpp
)

set_property(TARGET rigid-body-simulation PROPERTY CXX_STANDARD 11)
target_compile_options(rigid-body-simulation PRIVATE -Wall)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew-cmake EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)

target_link_libraries(rigid-body-simulation
    PRIVATE glfw
    PRIVATE libglew_static      # where does libglew_static come from?
    PRIVATE glm
)

target_include_directories(rigid-body-simulation
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)