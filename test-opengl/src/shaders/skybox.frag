#version 330
in vec3 skybox_coord0;
out vec4 frag_color;

uniform samplerCube g_sampler_skybox;

void main(){
	frag_color = texture(g_sampler_skybox, skybox_coord0);
}