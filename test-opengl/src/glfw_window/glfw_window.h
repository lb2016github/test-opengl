#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#ifdef _DLL_EXPORT_DECL
	#define DLL_API_DECL __declspec(dllexport)
#else
	#define DLL_API_DECL __declspec(dllimport)
#endif


#include <glfw\include\glfw3.h>

namespace glfw_win{

// ���̲����Ļص�
DLL_API_DECL void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// ����
DLL_API_DECL bool update();

// ����Ļص�
DLL_API_DECL void error_callback(int error, const char* description);

// ��ʼ������
DLL_API_DECL bool init_window(float width, float height, char* title);

// �ر�
DLL_API_DECL void destroy_window();

// ���ش��ڱ���
DLL_API_DECL float get_win_width();
DLL_API_DECL float get_win_height();

// ����ʱ��
DLL_API_DECL float get_time();
}
#endif

