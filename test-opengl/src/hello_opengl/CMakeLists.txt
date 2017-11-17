aux_source_directory(. HELLO_OPENGL_SRC)

file(GLOB_RECURSE HEADERS *.h *.hpp)

# add glad
set(GLAD_SRC ${PROJECT_SOURCE_DIR}/libs/glad/glad.h ${PROJECT_SOURCE_DIR}/libs/glad.c)

# 生成链接库
add_library(hello_opengl ${HELLO_OPENGL_SRC} ${HEADERS} ${GLAD_SRC})