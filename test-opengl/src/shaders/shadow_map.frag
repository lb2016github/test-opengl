#version 330
in vec2 tex_coord_out;
uniform sampler2D g_shadow_map;


out vec4 frag_color;

void main(){
	float depth = texture2D(g_shadow_map, tex_coord_out).x;
	depth = 1 - (1 - depth) * 25;
	frag_color = vec4(depth);
}