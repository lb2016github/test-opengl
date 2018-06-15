#version 410 core

layout(location=0) in vec3 position;
layout(location=1) in vec2 tex_coord;
layout(location=2) in vec3 normal;

uniform mat4 g_world;

struct VSOut{
	vec3 w_position;
	vec2 tex_coord;
	vec3 w_normal;
};

out VSOut vs_out;

void main(){
	vs_out.w_position = (g_world * vec4(position, 1)).xyz;
	vs_out.tex_coord = tex_coord;
	vs_out.w_normal = (g_world * vec4(normal, 1)).xyz;
}