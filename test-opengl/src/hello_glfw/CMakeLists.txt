# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

project(glfw_window)

set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../../libs/glfw/include")
set(LINK_DIR "${PROJECT_SOURCE_DIR}/../../libs/glfw")

include_directories ("${INCLUDE_DIR}")
link_directories("${LINK_DIR}")

# 显示头文件
file(GLOB_RECURSE CURRENT_HEADERS *.h *.hpp)
source_group("Include" FILES ${CURRENT_HEADERS})

# 查找当前目录下的所有源文件
# 并将名称保存到DIR_SRCS_GLFW变量
aux_source_directory(. DIR_SRCS_GLFW)

# 生成连接库
add_library(glfw_window SHARED ${DIR_SRCS_GLFW} ${CURRENT_HEADERS})

message("=============${CURRENT_HEADERS}")
message("=============${DIR_SRCS_GLFW}")

# 添加链接库
# target_link_libraries(glfw_window, glfw3)