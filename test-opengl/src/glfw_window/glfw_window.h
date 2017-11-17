#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H
#include <glfw\include\glfw3.h>

// ���̲����Ļص�
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// ����
void update(float delta_time);

// ����Ļص�
void error_callback(int error, const char* description);

// ��ʼ������
bool init_window(float width, float height, char* title);

// �ر�
void destroy_window();

// ���ش��ڱ���
float get_ratio();

#endif

