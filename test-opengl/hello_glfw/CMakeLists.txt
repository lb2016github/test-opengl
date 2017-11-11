# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

include_directories ("${PROJECT_SOURCE_DIR}/3rdparts/glfw")

# 查找当前目录下的所有源文件
# 并将名称保存到DIR_SRCS_GLFW变量
aux_source_directory(. DIR_SRCS_GLFW)

# 生成连接库
add_library(glfw_window ${DIR_SRCS_GLFW})

# 添加链接库
target_link_libraries(glfw_window, glfw3)