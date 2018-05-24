#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;

out vec2 tex_coord0;
out vec3 w_pos0;
out vec3 w_normal0;

uniform mat4 wvp;
uniform mat4 w;

void main(){
	gl_Position = wvp * vec4(position, 1);
	tex_coord0 = tex_coord;
	w_pos0 = (w * vec4(position, 1)).xyz;
	w_normal0 = (w * vec4(position, 0)).xyz;

}