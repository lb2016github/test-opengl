#include "glfw_window\glfw_window.h"
#include <stdio.h>
#include <thread>
#include <stdlib.h>
#include "practice_ogl\tutorial_practice.h"
#include "practice_ogl/tutorial_point_light.h"
#include "practice_ogl/tutorial_spot_light.h"
#include "practice_ogl/tutorial_shadow_map.h"
#include "practice_ogl/tutorial_normal_map.h"
#include "practice_ogl/tutorial_billboard.h"
#include "practice_ogl/tutorial_partical_system.h"
#include "practice_ogl/tutorial_picking.h"
#include "practice_ogl/tutorial_tessellation.h"
#include "practice_ogl/tutorial_instanced_render.h"
#include "practice_ogl/tutorial_deferred_shading.h"
#include "practice_ogl/tutorial_stencil_shadow.h"
#include "practice_ogl/tutorial_pbr.h"

void main(){
	fprintf(stdout, "%s\n", "HelloGLFW");
	bool rst = glfw_win::init_window(WINDOW_WIDTH, WINDOW_HEIGHT, "HellogLFW");
	if (!rst) {
		printf("init failed");
		glfw_win::destroy_window();
		return;
	}
	//TutorialPractice* tp = new TutorialPractice();
	//TutorialShadowMap *tp = new TutorialShadowMap();
	//TutorialParticalSystem* tp = new TutorialParticalSystem();
	//TutorialNormalMap* tp = new TutorialNormalMap();
	//TutorialPicking* tp = new TutorialPicking();
	//TutorialTessellation * tp = new TutorialTessellation();
	//TutorialInstancedRendering* tp = new TutorialInstancedRendering();
	//TutorialDeferredShading* tp = new TutorialDeferredShading();
	//TutorialStencilShadow* tp = new TutorialStencilShadow();
	TutorialPBR* tp = new TutorialPBR();
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
