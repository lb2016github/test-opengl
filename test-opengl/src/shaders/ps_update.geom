#version 330
layout(points) in;
layout(points, max_vertices=30) out;

struct Point{
	float type;
	float age;
	vec3 position;
	vec3 velocity;
};

in Point vs_out[];
out Point gs_out;

uniform float g_time;		// 当前时间
uniform float g_delta_time;	// 时间间隔
uniform float g_lancher_life_time;
uniform float g_shell_life_time;
uniform float g_secondary_shell_life_time;
uniform sampler1D g_random_texture;

#define PARTICAL_TYPE_LAUCHER 0.0f
#define PARTICAL_TYPE_SHELL 1.0f
#define PARTICAL_TYPE_SECONDARY_SHELL 2.0f

vec3 get_random_dir(float tex_coord){
	vec3 dir = texture(g_random_texture, tex_coord).xyz;
	dir -= vec3(0.5, 0.5, 0.5);	// [0, 1]->[-0.5, 0.5]
	return dir;
}

void main(){
	gs_out = vs_out[0];

	gs_out.age += g_delta_time;
	if (gs_out.type == PARTICAL_TYPE_LAUCHER){
		if(gs_out.age >= g_lancher_life_time){
			// 发射粒子
			gs_out.type = PARTICAL_TYPE_SHELL;
			gs_out.age = 0;
			vec3 dir = get_random_dir(g_time / 1000.0f);
			dir.y = max(dir.y, 0.5);
			gs_out.velocity = normalize(dir) / 20.0;
			EmitVertex();
			EndPrimitive();
			gs_out.age = 0;
		}
		gs_out.type = PARTICAL_TYPE_LAUCHER;
		gs_out.velocity = vs_out[0].velocity;
		EmitVertex();
		EndPrimitive();
	}
	else{
		float delta_time_sec = g_delta_time / 1000.0;
		vec3 delta_pos = gs_out.velocity * delta_time_sec;
		vec3 delta_v = (0, -9.8, 0) * vec3(delta_time_sec);

		if(gs_out.type == PARTICAL_TYPE_SHELL){
			if(gs_out.age >= g_shell_life_time){
				// 发射10个粒子
				for(int i = 0; i < 10; ++i){
					gs_out.type = PARTICAL_TYPE_SECONDARY_SHELL;
					gs_out.age = 0;
					vec3 dir = get_random_dir((g_time + i)/ 1000);
					gs_out.velocity = normalize(dir) / 20;
					EmitVertex();
					EndPrimitive();
				}
			}
			else{
				gs_out.position += delta_pos;
				gs_out.velocity += delta_v;
				EmitVertex();
				EndPrimitive();
			}
		}
		else{
			if(gs_out.age < g_secondary_shell_life_time){
				gs_out.position += delta_pos;
				gs_out.velocity += delta_v;
				EmitVertex();
				EndPrimitive();
				}
		}
		// else point消失
	}
}