#version 330
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 tex_coord;

struct VS_OUT{
	vec2 tex_coord;
};

out VS_OUT vs_out;

uniform mat4 wvp;

void main(){
	gl_Position = wvp * vec4(position, 1);
	vs_out.tex_coord = tex_coord;
}