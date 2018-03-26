#include <glad\glad.h>
#define _DLL_EXPORT_DECL
#include <glfw_window\glfw_window.h>
#include <stdio.h>


namespace glfw_win {

// window∂‘œÛ
GLFWwindow* window;
float width;
float height;

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

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);
	return true;
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
	glfwSwapBuffers(window);
	glfwPollEvents();
	return true;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	printf("on key callback %d\n", key);
}

void error_callback(int error, const char* description) {
	printf("on error callback %s\n", description);
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
