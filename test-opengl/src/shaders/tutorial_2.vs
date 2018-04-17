#version 330 core
layout (location = 0) in vec3 position;
uniform float scale;
uniform mat3 rot_mat; 
out vec4 color;

void main(){
	vec3 pos = rot_mat * position;
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	color = vec4(clamp(position, 0.0, 1.0), 1.0);
}