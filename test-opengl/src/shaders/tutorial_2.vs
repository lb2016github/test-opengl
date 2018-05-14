#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coor0;
uniform float scale;
uniform mat3 rot_mat; 

out vec2 o_tex_coor0;

void main(){
	vec3 pos = rot_mat * position;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	o_tex_coor0 = tex_coor0;
}