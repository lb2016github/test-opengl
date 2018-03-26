#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#ifdef _DLL_EXPORT_DECL
	#define DLL_API_DECL __declspec(dllexport)
#else
	#define DLL_API_DECL __declspec(dllimport)
#endif


#include <glfw\include\glfw3.h>

namespace glfw_win{

// 键盘操作的回调
DLL_API_DECL void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// 更新
DLL_API_DECL bool update();

// 错误的回调
DLL_API_DECL void error_callback(int error, const char* description);

// 初始化函数
DLL_API_DECL bool init_window(float width, float height, char* title);

// 关闭
DLL_API_DECL void destroy_window();

// 返回窗口比例
DLL_API_DECL float get_win_width();
DLL_API_DECL float get_win_height();

// 返回时间
DLL_API_DECL float get_time();
}
#endif

