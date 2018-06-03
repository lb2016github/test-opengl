#version 330 core

const int MAX_POINT_LIGHTS = 2;
const int MAX_SPOT_LIGHTS = 2;

in vec2 tex_coord0;
in vec3 w_pos0;
in vec3 w_normal0;
in vec4 shadow_map_coord0;

out vec4 frag_color;

struct BaseLight{
	vec3 color;
	float ambiant_intensity;
	float diffuse_intensity;
};

struct DirectionLight{
	BaseLight base_light;
	vec3 direction;
};

// 衰减参数
struct Attenuation{
	float constant;
	float linear;
	float expo;
};

struct PointLight{
	BaseLight base_light;
	vec3 position;
	Attenuation atten;
};

struct SpotLight{
	PointLight point_light;
	vec3 direction;
	float cutoff;	// dot小于cutoff的时候，不显示
};

uniform DirectionLight g_dir_light;
uniform PointLight g_point_light_list[MAX_POINT_LIGHTS];
uniform int g_point_light_num;
uniform SpotLight g_spot_light_list[MAX_SPOT_LIGHTS];
uniform int g_spot_light_num;

uniform vec3 g_eye_w_pos;
uniform sampler2D g_sampler;
uniform float g_mat_specular_intensity;
uniform float g_specular_power;
uniform sampler2D g_sampler_shadow_map;

vec3 calc_dir_color(vec3 light_dir, vec3 normal, BaseLight base_light){
	vec3 ambiant_color = base_light.color * base_light.ambiant_intensity;
	vec3 diffuse_color = vec3(0, 0, 0);
	vec3 specular_color = vec3(0, 0, 0);

	float diffuse_factor = dot(normal, -light_dir);
	if (diffuse_factor > 0){
		diffuse_color = base_light.color * base_light.diffuse_intensity;

		vec3 light_ref_dir = normalize(reflect(light_dir, normal));
		vec3 eye_dir = normalize(g_eye_w_pos - w_pos0);
		float specular_factor = dot(eye_dir, light_ref_dir);
		if (specular_factor > 0){
			specular_factor = pow(specular_factor, g_specular_power);
			specular_color = base_light.color * g_mat_specular_intensity * specular_factor;
		}
	}

	return (ambiant_color + diffuse_color + specular_color);
}

vec3 calc_point_color(PointLight light, vec3 normal){
	vec3 dir = w_pos0 - light.position;
	float dis = length(dir);
	dir = normalize(dir);
	vec3 color = calc_dir_color(dir, normal, light.base_light);
	// 计算衰减
	float atten = light.atten.constant + light.atten.linear * dis + light.atten.expo * dis * dis;;
	color = color / atten;
	return color;
}

vec3 calc_spot_color(SpotLight light, vec3 normal){
	vec3 color = vec3(0, 0, 0);
	vec3 pix_dir = normalize(w_pos0 - light.point_light.position);
	float dot_rst = dot(pix_dir, light.direction);
	if (dot_rst > light.cutoff){
		color = calc_point_color(light.point_light, normal);
		// [cutoff, 1]映射到[0, 1]
		float factor = (dot_rst - light.cutoff) / (1 - light.cutoff);
		color *= factor;
	}
	return color;
}

void main(){
	vec3 normal = normalize(w_normal0);
	vec3 v_shadow = shadow_map_coord0.xyz / shadow_map_coord0.w;
	float shadow_u = 0.5 * v_shadow.x + 0.5;
	float shadow_v = 0.5 * v_shadow.y + 0.5;
	float shadow_depth = 0.5 * v_shadow.z +0.5;

	// shadow map
	float depth = texture2D(g_sampler_shadow_map, vec2(shadow_u, shadow_v)).x;
	float factor = 1;
	if (shadow_depth > depth){
		factor = 0.5;
	}

	// 计算平行光颜色
	vec3 sum_color = calc_dir_color(g_dir_light.direction, normal, g_dir_light.base_light);
	// 计算点光源颜色
	for(int i = 0; i < g_point_light_num; i++){
		sum_color += calc_point_color(g_point_light_list[i], normal);
	}
	// 计算spot light
	for(int i = 0; i < g_spot_light_num; i++){
		sum_color += calc_spot_color(g_spot_light_list[i], normal) * factor;
	}
	// 计算最终的颜色
	frag_color = texture2D(g_sampler, tex_coord0) * vec4(sum_color, 1);
}