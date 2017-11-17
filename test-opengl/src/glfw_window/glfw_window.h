#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H
#include <glfw\include\glfw3.h>

namespace glfw_win{

// 键盘操作的回调
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// 更新
bool update();

// 错误的回调
void error_callback(int error, const char* description);

// 初始化函数
bool init_window(float width, float height, char* title);

// 关闭
void destroy_window();

// 返回窗口比例
float get_win_width();
float get_win_height();

// 返回时间
float get_time();
}
#endif

