#version 330
layout (location=0) in vec3 positon;

out vec3 skybox_coord0;

uniform mat4 wvp;

void main(){
	vec4 wvp_pos = wvp * vec4(positon, 1);
	gl_Position = wvp_pos.xyww;
	skybox_coord0 = positon;
}
