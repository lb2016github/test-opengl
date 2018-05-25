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
link_directories(./libs/assimp)


set(GLAD_SRC "libs/glad/glad.h" "libs/glad.c")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_RELEASE_POSTFIX "_r")

file(GLOB_RECURSE SHADER_SRC src/shaders/*.*)
source_group(shaders FILES ${SHADER_SRC})

#================================ Start 添加子工程 ================================#
# glad
add_library(glad STATIC ${GLAD_SRC})

# 添加窗口工程
file(GLOB_RECURSE WINDOW_SRC src/glfw_window/*.*)
add_library(ogl_window STATIC ${WINDOW_SRC})
target_link_libraries(ogl_window glfw3 glad)

# # 添加util工程
# file(GLOB_RECURSE UTIL_SRC src/util/*.*)
# add_library(util STATIC ${UTIL_SRC})
# target_link_libraries(util glad)

# 添加数学库
file(GLOB_RECURSE MATH3D_SRC src/math3d/*.*)
add_library(math3d STATIC ${MATH3D_SRC})

# 添加practice_ogl
file(GLOB_RECURSE PRACTICE_SRC src/practice_ogl/*.* src/stb-master/*.*)
add_executable(practice_ogl ${PRACTICE_SRC} src/main.cpp ${SHADER_SRC})
target_link_libraries(practice_ogl ogl_window math3d)
set_target_properties(practice_ogl PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(practice_ogl PROPERTIES RELEASE_POSTFIX "_r")

# 移动shader文件
add_custom_target(
	copy_shader
	ALL
	COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin/shaders
	COMMAND ${CMAKE_COMMAND} -E copy ${SHADER_SRC} ${CMAKE_BINARY_DIR}/bin/shaders
	)

#================================ End 添加子工程 ================================#
