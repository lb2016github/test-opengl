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

# include文件
file(GLOB_RECURSE HEADERS src/glfw_window/*.h src/glfw_window/*.hpp)
#source_group("Include" FILES {HEADERS})

# 查找当前目录下的所有源文件
# 并将名称保存到DIR_SCRS变量
aux_source_directory(./src DIR_SRCS)
aux_source_directory(./src/glfw_window GLFW_WINDOW_SRCS)
SET(GLAD_SOURCE "libs/glad/glad.h" "libs/glad.c")
SET(SRC ${DIR_SRCS} ${GLFW_WINDOW_SRCS} ${HEADERS} ${GLAD_SOURCE})

MESSAGE(${SRC})

# 添加子目录
add_subdirectory(src/hello_opengl)
link_directories(src/hello_opengl)

# 指定生成目标
add_executable(test-opengl ${SRC})

# 链接库
target_link_libraries(test-opengl glfw3 hello_opengl)
