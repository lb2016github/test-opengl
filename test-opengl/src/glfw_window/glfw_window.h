#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H
#include <glfw\include\glfw3.h>

// 键盘操作的回调
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// 更新
void update(float delta_time);

// 错误的回调
void error_callback(int error, const char* description);

// 初始化函数
bool init_window(float width, float height, char* title);

// 关闭
void destroy_window();

// 返回窗口比例
float get_ratio();

#endif

