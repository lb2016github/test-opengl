#pragma once
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>

class GLFWWindowBase {
public:
	// 错误的回调
	virtual void errorCallback(int error, const char* description) { fprintf(stderr, "Error: %s\n", description); };

	// 更新
	virtual void update(float delta_time) {};
	///////////////////////////////////////////////////////////////////////////
	GLFWWindowBase(int width, int height, const char* title, GLFWkeyfun keyCallback, );

	virtual ~GLFWWindowBase();
protected:
	GLFWwindow* window;

};


GLFWWindowBase::GLFWWindowBase(int width, int height, const char* title, GLFWkeyfun keyCallback) {
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {	// 创建失败
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, keyCallback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
}

GLFWWindowBase::~GLFWWindowBase() {
	if (!window) {
		return;
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}