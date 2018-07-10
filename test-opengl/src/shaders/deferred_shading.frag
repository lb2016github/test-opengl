#version 330

layout(location=0) out vec3 w_pos;
layout(location=1) out vec3 w_normal;
layout(location=2) out vec3 diffuse;
layout(location=3) out vec3 tex_coord;

struct VSOut{
	vec3 w_pos;
	vec3 w_normal;
	vec2 tex_coord;
};

in VSOut vs_out;

uniform sampler2D g_color_sampler;

void main(){
	w_pos = vs_out.w_pos;
	w_normal = vs_out.w_normal;
	tex_coord = vec3(vs_out.tex_coord, 0);
	diffuse = texture(g_color_sampler, vs_out.tex_coord).xyz;
}

