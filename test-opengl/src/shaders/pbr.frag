#version 330 core
#define MAX_LIGHT_NUM 5
#define PI 3.1415926
struct PointLight{
	vec3 pos;
	vec3 color;
};

struct FragIn{
	vec3 coord;
	vec3 w_pos;
	vec3 w_normal;
};

uniform sampler1D g_tex_roughness;	// 粗糙度贴图
uniform sampler1D g_tex_metalness;	// 金属度贴图
uniform sampler3D g_tex_albedo;	// 基础发射率贴图（即diffuse，只是概念上不同）

uniform PointLight g_point_lights[MAX_LIGHT_NUM];	// 点光源
uniform int g_light_num;		// 光照数目
uniform vec3 g_cam_pos;			// 相机位置

in FragIn vs_in;			// 输入
out vec4 g_frag_color;	// 输出颜色

float distribution_ggx(vec3 n, vec3 h, float roughness);
vec3 fresnel_schlick(float h_o_v, vec3 f0);
float geometry_schlick_ggx(float costheta, float roughness);
float geometry_smith(vec3 n, vec3 l, vec3 v, float roughness);
float calc_attenuation(float distance);	// 计算光照衰减

void main(){
	vec3 w_normal = normalize(vs_in.w_normal);	// 表面normal方向
	vec3 w_pos = vs_in.w_pos;
	float roughness = sample(g_tex_roughness, vs_in.coord);
	float metalness = sample(g_tex_metalness, vs_in.coord);
	vec3 albedo = sample(g_tex_albedo, vs_in.coord);
	vec3 view_dir = normalize(g_cam_pos - w_pos);	// 相机朝向

	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, metalness);

	vec3 color = vec3(0, 0, 0);
	float light_num = min(g_light_num, MAX_LIGHT_NUM);
	for(int i = 0; i < light_num; ++i){
		PointLight light = g_point_lights[i];
		vec3 light_dir = normalize(light.pos - w_pos);		// 光照方向
		vec3 h_dir = normalize(view_dir + light_dir);		// h方向

		// 光照计算
		float D = distribution_ggx(w_normal, h_dir, roughness);
		vec3 F = fresnel_schlick(dot(h_dir, view_dir), f0);
		float G = geometry_smith(w_normal, light_dir, view_dir, roughness);
//		float dfg_denom = 4 * dot(w_normal, view_dir) * dot(w_normal, light_dir);
		float dfg_denom = 4 * max(dot(w_normal, view_dir), 0) * max(dot(w_normal, light_dir), 0) + 0.001;
		vec3 specular = D * F * G / dfg_denom;

		vec3 ks = F;
		vec3 kd = vec3(1) - ks;
		kd *= 1.0 - metalness;	// 金属不会折射光线（会直接吸收）

		vec3 radiance = light.color * calc_attenuation(length(light.pos - w_pos));
		float n_dot_l = dot(w_normal, light_dir);

		color += kd * (albedo / PI + specular) * n_dot_l * radiance;
	}
	g_frag_color = vec4(color, 1);
}

float calc_attenuation(float distance){
	return 1 / distance / distance;
}

// 计算D项
float distribution_ggx(vec3 n, vec3 h, float roughness){
	float a = roughness * roughness;	// 据迪士尼的经验调整，使得光照看起来更加自然
	float a2 = a * a;
	float n_o_h = dot(n, v);
	float denom = n_o_h * n_o_h * (a2 - 1) + 1;
	denom = PI * denom * denom;
	return a2 / denom;
}

// 计算F项
vec3 fresnel_schlick(float h_o_v, vec3 f0){
	return f0 + (1 - f0) * pow(1 - h_o_v, 5);
}

// 计算G项(直接光照)
float geometry_schlick_ggx(float costheta, float roughness){
	float k = (roughness + 1);
	k = k * k/ 8;

	float denom = costheta * (1 - k) + k;

	return costheta / denom;
}

float geometry_smith(vec3 n, vec3 l, vec3 v, float roughness){
	float n_dot_l = max(dot(n, l), 0);
	float n_dot_v = max(dot(n, v), 0);
	return geometry_schlick_ggx(n_dot_l, roughness) * geometry_schlick_ggx(n_dot_v, roughness);
}


