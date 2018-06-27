#version 410 core
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
layout(location=LOCATION_WVP_INDEX) in mat4 wvp;
layout(location=LOCATION_WORLD_INDEX) in mat4 world;

struct VSOut{
	vec2 tex_coord;
	vec3 normal;
};

out VSOut vs_out;

void main(){
	gl_Position = wvp * vec4(position, 1);
	vs_out.tex_coord = tex_coord;
	vec4 world_normal = world * vec4(normal, 0);
	vs_out.normal = normalize(world_normal.xyz);
}