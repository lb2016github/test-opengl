#version 330 core
#define LOCATION_POSITON_INDEX 0
#define LOCATION_TEX_COORD_INDEX 1
#define LOCATION_NORMAL_INDEX 2
#define LOCATION_TANGENT_INDEX 3
#define LOCATION_WVP_INDEX 10
#define LOCATION_WORLD_INDEX 14

layout(location=LOCATION_POSITON_INDEX) in vec3 position;
layout(location=LOCATION_TEX_COORD_INDEX) in vec2 tex_coord;
layout(location=LOCATION_NORMAL_INDEX) in vec3 normal;



struct VSOut{
	vec2 coord;
	vec3 w_pos;
	vec3 w_normal;
};

uniform mat4x4 g_mvp;
uniform mat4x4 g_world;

out VSOut vs_out;

void main(){
	gl_Position = g_mvp * vec4(position, 1);
	vs_out.coord = tex_coord;
	vs_out.w_pos = (g_world * vec4(position, 1)).xyz;
	vs_out.w_normal = (g_world * vec4(normal, 1)).xyz;
}