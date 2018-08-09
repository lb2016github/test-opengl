#version 410
layout(location=0) in vec3 position;

uniform mat4 g_world_trans;

struct VSOut{
	vec3 w_position;
};

out VSOut vs_out;

void main(){
	vs_out.w_position = (g_world_trans * vec4(position, 1)).xyz;
}