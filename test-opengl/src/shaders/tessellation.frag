#version 410 core

struct TESOut{
	vec2 tex_coord;
};

in TESOut tes_out;
out vec4 frag_color;

uniform sampler2D g_sampler_color_map;

void main(){
	frag_color = texture(g_sampler_color_map, tes_out.tex_coord);
}