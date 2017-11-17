#include <glad\glad.h>
#include <glfw_window\glfw_window.h>
#include <stdio.h>

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

void update(float delta_time) {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	printf("on key callback %d\n", key);
}

void error_callback(int error, const char* description) {
	printf("on error callback %s\n", description);
}

float get_ratio() {
	return width / height;
}