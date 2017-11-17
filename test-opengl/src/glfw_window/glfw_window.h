#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H
#include <glfw\include\glfw3.h>

namespace glfw_win{

// ���̲����Ļص�
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// ����
bool update();

// ����Ļص�
void error_callback(int error, const char* description);

// ��ʼ������
bool init_window(float width, float height, char* title);

// �ر�
void destroy_window();

// ���ش��ڱ���
float get_win_width();
float get_win_height();

// ����ʱ��
float get_time();
}
#endif

