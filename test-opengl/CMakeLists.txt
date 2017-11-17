# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project(test-opengl)

add_subdirectory("${PROJECT_SOURCE_DIR}/src/hello_glfw")

# 加入一个配置头文件，用于处理CMake对源码的设置
configure_file(
"${PROJECT_SOURCE_DIR}/config.h.in"
"${PROJECT_BINARY_DIR}/config.h"
)

# 设置宏
option(USE_TEST "use test" ON)

include_directories ("${PROJECT_SOURCE_DIR}/src/hello_glfw")
link_directories("${PROJECT_SOURCE_DIR}/src/hello_glfw")

set (EXTRA_LIBS ${EXTRA_LIBS} hello_glfw)

# 查找当前目录下的所有源文件
# 并将名称保存到DIR_SCRS变量
aux_source_directory(./src DIR_SRCS)


# 指定生成目标
add_executable(test-opengl ${DIR_SRCS})

target_link_libraries(test-opengl ${EXTRA_LIBS})