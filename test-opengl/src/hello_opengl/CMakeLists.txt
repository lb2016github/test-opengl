set(HELLO_OPENGL_SRC hello_opengl.h hello_opengl_2.cpp)

# add glad
set(GLAD_SRC ${PROJECT_SOURCE_DIR}/libs/glad/glad.h ${PROJECT_SOURCE_DIR}/libs/glad.c)

# 生成链接库
add_library(hello_opengl ${HELLO_OPENGL_SRC} ${GLAD_SRC})