#define HELLOGLFWDLL_EXPORTS
#include "HelloGLFW.h"

HelloGLFW::HelloGLFW(int width, int height, const char* title): GLFWWindowBase(width, height, title)
{
	
}


HelloGLFW::~HelloGLFW()
{
}


// ���̲����Ļص�
void HelloGLFW::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GLFWWindowBase::keyCallback(window, key, scancode, action, mods);
}

// ����
void HelloGLFW::update(float delta_time) {
	GLFWWindowBase::update(delta_time);
}