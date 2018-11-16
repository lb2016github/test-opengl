#version 330 core
layout(location=0) in vec3 position;


struct VSOut{
	vec3 coord;
	vec3 w_pos;
	vec3 w_normal;
};

uniform mat4x4 g_mvp;

out VSOut vs_out;

void main(){
	gl_Position = 
}