# Porjects Settings
cmake_minimum_required(VERSION 3.1)
project(rigid-body-simulation)

find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

add_executable(rigid-body-simulation
    src/fps.cpp
    src/util.cpp
    src/main.cpp
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)

add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
add_subdirectory(lib/glew-cmake EXCLUDE_FROM_ALL)
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
add_subdirectory(lib/glfreetype EXCLUDE_FROM_ALL)

target_link_libraries(rigid-body-simulation
    PUBLIC glfreetype_lib 
    PUBLIC ${OPENGL_LIBRARIES}
    PUBLIC ${FREETYPE_LIBRARIES}
    PUBLIC glfw
    PUBLIC libglew_static      # where does libglew_static come from?
    PUBLIC glm
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
)