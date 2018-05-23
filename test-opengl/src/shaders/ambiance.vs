#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coor0;
layout (location = 2) in vec3 normal;
uniform mat4 wvp;
uniform mat4 w;

out vec2 o_tex_coor0;
out vec3 o_normal;

void main(){
	gl_Position = wvp * vec4(position, 1);
	o_tex_coor0 = tex_coor0;
	o_normal = (w * vec4(normal, 0)).xyz;
}