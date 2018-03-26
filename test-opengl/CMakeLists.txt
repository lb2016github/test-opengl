# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project(test-opengl)

cmake_policy(SET CMP0015 NEW)

# 加入一个配置头文件，用于处理CMake对源码的设置
configure_file(
"${PROJECT_SOURCE_DIR}/config.h.in"
"${PROJECT_BINARY_DIR}/config.h"
)

# 设置宏
option(USE_TEST "use test" ON)

include_directories(libs)
include_directories(src)
link_directories(./libs/glfw)


set(GLAD_SRC "libs/glad/glad.h" "libs/glad.c")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#================================ Start 添加子工程 ================================#
# glad
add_library(glad ${GLAD_SRC})

# 添加窗口工程
file(GLOB_RECURSE WINDOW_SRC src/glfw_window/*.*)
add_library(ogl_window ${WINDOW_SRC})
target_link_libraries(ogl_window glfw3 glad)

# 添加practice_ogl
file(GLOB_RECURSE PRACTICE_SRC src/practice_ogl/*.*)
add_executable(practice_ogl ${PRACTICE_SRC} src/main.cpp)
target_link_libraries(practice_ogl ogl_window)

# 添加hello_opengl
set(HELLO_OPENGL_SRC src/hello_opengl/hello_opengl.cpp src/main.cpp)
add_executable(hello_ogl ${HELLO_OPENGL_SRC})
target_link_libraries(hello_ogl ogl_window)

#================================ End 添加子工程 ================================#
