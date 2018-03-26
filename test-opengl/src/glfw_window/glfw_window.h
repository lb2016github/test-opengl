#ifndef GLFW_WINDOW_H
#define GLFW_WINDOW_H

#include <glfw\include\glfw3.h>

namespace glfw_win{

// ¼üÅÌ²Ù×÷µÄ»Øµ÷
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// ¸üÐÂ
bool update();

// ´íÎóµÄ»Øµ÷
void error_callback(int error, const char* description);

// ³õÊ¼»¯º¯Êý
bool init_window(float width, float height, char* title);

// ¹Ø±Õ
void destroy_window();

// ·µ»Ø´°¿Ú±ÈÀý
float get_win_width();
float get_win_height();

// ·µ»ØÊ±¼ä
float get_time();
}
#endif

