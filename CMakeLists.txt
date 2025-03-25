cmake_minimum_required(VERSION 3.26.0)
project(my_renderer_batoo VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(my_renderer_batoo
    src/config.h
    src/main.cpp
    src/glad.c
    src/triangle_mesh.h
    src/triangle_mesh.cpp
    src/shader_loader.h
    src/shader_loader.cpp
    src/sea.h
    src/sea.cpp
    src/sky.h
    src/sky.cpp
    src/camera.h
    src/camera.cpp
)

target_include_directories(my_renderer_batoo PRIVATE dependencies)

target_link_libraries(my_renderer_batoo ${GLFW3_LIBRARY} OpenGL::GL)
