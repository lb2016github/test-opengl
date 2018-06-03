#include "glfw_window\glfw_window.h"
#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include "practice_ogl\tutorial_practice.h"
#include "practice_ogl/tutorial_point_light.h"
#include "practice_ogl/tutorial_spot_light.h"
#include "practice_ogl/tutorial_shadow_map.h"

void main(){
	fprintf(stdout, "%s\n", "HelloGLFW");
	bool rst = glfw_win::init_window(WINDOW_WIDTH, WINDOW_HEIGHT, "HellogLFW");
	if (!rst) {
		printf("init failed");
		glfw_win::destroy_window();
		return;
	}
	
	TutorialShadowMap* tp = new TutorialShadowMap();
	//TutorialSpotLight* tp = new TutorialSpotLight();
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
