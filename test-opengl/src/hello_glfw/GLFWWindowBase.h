#pragma once
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <stdlib.h>

class GLFWWindowBase {
public:
	// ���̲����Ļص�
	virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	// ����Ļص�
	virtual void errorCallback(int error, const char* description) { fprintf(stderr, "Error: %s\n", description); };

	// ����
	virtual void update(float delta_time) {};
	///////////////////////////////////////////////////////////////////////////
	GLFWWindowBase(int width, int height, const char* title);

	~GLFWWindowBase();
protected:
	GLFWwindow* window;

};


GLFWWindowBase::GLFWWindowBase(int width, int height, const char* title) {
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {	// ����ʧ��
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetKeyCallback(window, this->keyCallback);

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

void GLFWWindowBase::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}