#pragma once
#ifdef HELLOGLFWDLL_EXPORTS
#define HELLOGLFWDLL_API __declspec(dllexport)
#else
#define HELLOGLFWDLL_API __declspec(dllimport)
#endif

#include"glfw3.h"
#include "GLFWWindowBase.h"

class HELLOGLFWDLL_API HelloGLFW: GLFWWindowBase
{
public:
	HelloGLFW(int width, int height, const char* title);

	~HelloGLFW();

/*
vitural functions
*/
public:
	// ���̲����Ļص�
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// ����
	void update(float delta_time);

};

