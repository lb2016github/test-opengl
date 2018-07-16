#version 330 core

const int MAX_POINT_LIGHTS = 2;

in vec2 tex_coord0;
in vec3 w_pos0;
in vec3 w_normal0;

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

uniform DirectionLight g_dir_light;
uniform PointLight g_point_light_list[MAX_POINT_LIGHTS];
uniform int g_point_light_num;
uniform vec3 g_eye_w_pos;
uniform sampler2D g_sampler;
uniform float g_mat_specular_intensity;
uniform float g_specular_power;

vec3 calc_dir_color(vec3 light_dir, vec3 normal, BaseLight base_light){
	vec3 ambiant_color = base_light.color * base_light.ambiant_intensity;
	vec3 diffuse_color = vec3(0, 0, 0);
	vec3 specular_color = vec3(0, 0, 0);

	float diffuse_factor = dot(normal, -light_dir);
	if (diffuse_factor > 0){
		diffuse_color = base_light.color * base_light.diffuse_intensity * diffuse_factor;

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

float calc_attenuation(Attenuation atten, float dis){
	return atten.constant + atten.linear * dis + atten.expo * dis * dis;
}

void main(){
	vec3 normal = normalize(w_normal0);
	// 计算平行光颜色
	vec3 dir_color = calc_dir_color(g_dir_light.direction, normal, g_dir_light.base_light);
	// 计算点光源颜色
	vec3 point_color = vec3(0, 0, 0);
	for(int i = 0; i < g_point_light_num; i++){
		PointLight light = g_point_light_list[i];
		vec3 dir = w_pos0 - light.position;
		float dis = length(dir);
		dir = normalize(dir);
		vec3 tmp_color = calc_dir_color(dir, normal, light.base_light);
		// 计算衰减
		float atten = calc_attenuation(light.atten, dis);
		tmp_color = tmp_color / atten;
		point_color += tmp_color;
	}
	// 计算最终的颜色
	frag_color = texture2D(g_sampler, tex_coord0) * vec4(dir_color + point_color, 1);
}