#include "HelloGLFW.h"
#include <stdio.h>
#include <thread>
#include <stdlib.h>

void main(){
	fprintf(stdout, "%s\n", "HelloGLFW");
	HelloGLFW* window = new HelloGLFW(1344, 750, "Hello GLFW");
	while (true)
	{
		window->update(0.1);
		_sleep(0.1);
	}
	return;
}