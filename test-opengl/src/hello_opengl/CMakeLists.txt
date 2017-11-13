# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 查找当前目录下的所有源文件
# 并将名称保存到DIR_SRCS_TEST_OPENGL变量
aux_source_directory(. DIR_SRCS_TEST_OPENGL)

# 生成连接库
add_library(test_opengl ${DIR_SRCS_TEST_OPENGL})