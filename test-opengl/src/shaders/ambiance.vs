#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coor0;
uniform mat4 wvp;

out vec2 o_tex_coor0;

void main(){
	gl_Position = wvp * vec4(position, 1);
	o_tex_coor0 = tex_coor0;
}