#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H
#include <glad\glad.h>
#include "glfw\include\glfw3.h"
#include "common\i_callbacks.h"

namespace glfw_win{

// 键盘回调
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// 鼠标点击回调
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// 光标移动回调
void cursor_position_callback(GLFWwindow* window, double x, double y);

// 滚轮回调
void scroll_callback(GLFWwindow* window, double x, double y);

// 更新
bool update();

// 错误的回调
void error_callback(int error, const char* description);

// 初始化窗口
bool init_window(float width, float height, char* title);

// 设置回调类
void set_callback(ICallback* i_callback);

// 销毁窗口
void destroy_window();

// 获取窗口宽高
float get_win_width();
float get_win_height();

// 获取当前时间
float get_time();
}
#endif

