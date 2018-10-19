#include "glfw_window\glfw_window.h"
#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include "ray_tracing.h"

void main(){
	fprintf(stdout, "%s\n", "HelloGLFW");
	bool rst = glfw_win::init_window(WINDOW_WIDTH, WINDOW_HEIGHT, "HellogLFW");
	if (!rst) {
		printf("init failed");
		glfw_win::destroy_window();
		return;
	}
	RayTracingDemo* rtd = new RayTracingDemo();
	rtd->init();
	glfw_win::set_callback(rtd);
	while (true)
	{
		bool rst = glfw_win::update();
		if (!rst)
			break;
	}
	glfw_win::destroy_window();
	delete rtd;
	return;
}
