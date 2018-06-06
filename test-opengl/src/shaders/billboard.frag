#version 330
in vec2 tex_coord;
out vec4 frag_color;

uniform sampler2D g_color_map;

void main(){
	frag_color = texture2D(g_color_map, tex_coord);
	if (frag_color.r == 0 && frag_color.g == 0 && frag_color.b == 0){
		discard;
	}
}