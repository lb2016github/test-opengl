#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;

out struct{
	vec3 w_pos;
	vec3 w_normal;
	vec3 w_tangent;
	vec2 tex_coord;
	vec4 light_space_pos;
}v_out;


uniform mat4 wvp;
uniform mat4 w;
uniform mat4 light_wvp;

void main(){
	gl_Position = wvp * vec4(position, 1);
	v_out.tex_coord = tex_coord;
	v_out.w_pos = (w * vec4(position, 1)).xyz;
	v_out.w_normal = (w * vec4(normal, 0)).xyz;
	v_out.light_space_pos = light_wvp * vec4(position, 1);
	v_out.w_tangent = (w * vec4(tangent, 0)).xyz;
}