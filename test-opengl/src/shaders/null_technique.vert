#version 330

layout(location=0) in vec3 position;

uniform mat4 g_wvp;

void main(){
	gl_Position = g_wvp * vec4(position, 1); 
}