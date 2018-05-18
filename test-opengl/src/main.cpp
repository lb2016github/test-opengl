#include "glfw_window\glfw_window.h"
#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include "practice_ogl\tutorial_practice.h"

void main(){
	fprintf(stdout, "%s\n", "HelloGLFW");
	bool rst = glfw_win::init_window(640, 480, "HellogLFW");
	if (!rst) {
		printf("init failed");
		glfw_win::destroy_window();
		return;
	}
	
	TutorialPractice* tp = new TutorialPractice();
	tp->init();
	glfw_win::set_callback(tp);
	while (true)
	{
		bool rst = glfw_win::update();
		if (!rst)
			break;
	}
	glfw_win::destroy_window();
	delete tp;
	return;
}
