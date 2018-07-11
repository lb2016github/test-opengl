#version 330
// ±ê¼Ç¶¥µãlayout
#define LOCATION_POSITON_INDEX 0
#define LOCATION_TEX_COORD_INDEX 1
#define LOCATION_NORMAL_INDEX 2
#define LOCATION_TANGENT_INDEX 3
#define LOCATION_WVP_INDEX 10
#define LOCATION_WORLD_INDEX 14

layout(location=LOCATION_POSITON_INDEX) in vec3 position;
layout(location=LOCATION_TEX_COORD_INDEX) in vec2 tex_coord;
layout(location=LOCATION_NORMAL_INDEX) in vec3 normal;

uniform mat4 wvp;
uniform mat4 world;

struct VSOut{
	vec3 w_pos;
	vec3 w_normal;
	vec2 tex_coord;
};
out VSOut vs_out;

void main(){
	gl_Position = wvp * vec4(position, 1);
	vs_out.w_pos = (world * vec4(position, 1)).xyz;
	vs_out.w_normal = (world * vec4(normal, 1)).xyz;
	vs_out.tex_coord = tex_coord;
}