#include <util\practice_header.h>
#include <glfw_window\glfw_window.h>
#include <stdio.h>
#include <thread>
#include <stdlib.h>

void main(){
	fprintf(stdout, "%s\n", "HelloGLFW");
	bool rst = glfw_win::init_window(640, 480, "HellogLFW");
	if (!rst) {
		printf("init failed");
		glfw_win::destroy_window();
		return;
	}
	gl_init();
	while (true)
	{
		float width = glfw_win::get_win_width();
		float height = glfw_win::get_win_height();
		float time = glfw_win::get_time();
		gl_update(width, height, time);
		bool rst = glfw_win::update();
		if (!rst)
			break;
	}
	gl_destroy();
	glfw_win::destroy_window();
	return;
}