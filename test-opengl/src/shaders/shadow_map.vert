#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

out vec2 tex_coord_out;

uniform mat4 g_wvp;

void main(){
	gl_Position = g_wvp * vec4(position, 1);
	tex_coord_out = tex_coord;
}