#include <glfw_window\glfw_window.h>
#include <stdio.h>
#include "practice_ogl\i_callbacks.h"


namespace glfw_win {

// window对象
GLFWwindow* window;
float width;
float height;
ICallback* callback;

bool init_window(float width, float height, char* title) {
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(width, height, title, NULL, NULL);

	//init failed
	if (!window) {
		destroy_window();
		return false;
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	return true;
}

void set_callback(ICallback* i_callback) {
	callback = i_callback;
}

void destroy_window() {
	if (window) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}

bool update() {
	if (glfwWindowShouldClose(window))
		return false;
	int i_width, i_height;
	glfwGetFramebufferSize(window, &i_width, &i_height);
	width = i_width;
	height = i_height;
	float time = get_time();

	callback->render_scene_callback(width, height, time);

	glfwSwapBuffers(window);
	glfwPollEvents();
	return true;
}

// 按键回调
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//printf("on key callback %d\n", key);
	callback->key_callback(key, scancode, action, mods);
}

// 鼠标点击回调
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	//printf("todo: on mouse button callback %d %d %d\n", button, action, mods);
	callback->mouse_button_callback(button, action, mods);
}

// 光标移动回调
void cursor_position_callback(GLFWwindow* window, double x, double y) {
	//printf("todo: on cursor position callback %f %f\n", x, y);
	callback->cursor_position_callback(x, y);
}

// 滚轮回调
void scroll_callback(GLFWwindow* window, double x, double y) {
	//printf("todo: on scroll callback %f %f\n", x, y);
	callback->scroll_callback(x, y);
}

void error_callback(int error, const char* description) {
	//printf("on error callback %s\n", description);
	callback->error_callback(error, description);
	
}

float get_win_width() {
	return width;
}

float get_win_height() {
	return height;
}

float get_time() {
	return (float)glfwGetTime();
}

}
