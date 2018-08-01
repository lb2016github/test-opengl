#version 410

layout(location=0)in vec3 position;
layout(location=1)in vec3 normal;

uniform mat4 g_wvp;
uniform mat4 g_world_mat;

out vec3 world_pos;

void main(){
	gl_Position = g_wvp * vec4(position, 1);
	world_pos = (g_world_mat * vec4(position, 1)).xyz;
}