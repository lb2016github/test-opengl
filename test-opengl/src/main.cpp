#include <glfw_window\glfw_window.h>
#include <stdio.h>
#include <thread>
#include <stdlib.h>

void main(){
	fprintf(stdout, "%s\n", "HelloGLFW");
	init_window(1344, 750, "HellogLFW");
	while (true)
	{
		_sleep(0.1);
	}
	destroy_window();
	return;
}