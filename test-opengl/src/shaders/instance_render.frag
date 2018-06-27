#version 410 core

struct VSOut{
	vec2 tex_coord;
	vec3 normal;
};

in VSOut vs_out;
out vec4 frag_color;
uniform sampler2D g_sampler_color;

void main(){
	vec4 color = texture(g_sampler_color, vs_out.tex_coord);
	frag_color = color;
}