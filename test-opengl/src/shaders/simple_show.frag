#version 330

out vec4 frag_color;

struct VS_OUT{
	vec2 tex_coord;
};

in VS_OUT vs_out;

uniform sampler2D g_color_sampler;

void main(){
	frag_color = texture(g_color_sampler, vs_out.tex_coord);
//	if (frag_color.z > 0){
//		frag_color = vec4(1, 1, 1, 1);
//	}
}