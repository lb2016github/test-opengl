#version 330

const int MAX_POINT_LIGHTS = 2;

struct BaseLight{
	vec3 color;
	float ambiant_intensity;
	float diffuse_intensity;
};

// Ë¥¼õ²ÎÊý
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

out vec4 frag_color;

uniform sampler2D g_diffuse_sampler;
uniform sampler2D g_position_sampler;
uniform sampler2D g_normal_sampler;

uniform PointLight g_point_light_list[MAX_POINT_LIGHTS];
uniform int g_point_light_num;

uniform vec2 g_win_size;
uniform vec3 g_eye_w_pos;
uniform float g_mat_specular_intensity;
uniform float g_specular_power;

vec2 calc_tex_coord();
vec3 calc_dir_color(vec3 light_dir, vec3 normal, BaseLight base_light, float shadow_factor, vec3 world_pos);
vec3 calc_point_light_color(vec3 world_pos, vec3 normal, PointLight point_light);

void main(){
	vec2 tex_coord = calc_tex_coord();
	vec3 diffuse_color = texture(g_diffuse_sampler, tex_coord).xyz;
	vec3 position = texture(g_position_sampler, tex_coord).xyz;
	vec3 normal = texture(g_normal_sampler, tex_coord).xyz;
	normal = normalize(normal);
	vec3 light_color = vec3(0, 0, 0);
	for(int i = 0; i < g_point_light_num; ++i){
		light_color += calc_point_light_color(position, normal, g_point_light_list[i]);
	}
	frag_color = vec4(diffuse_color * light_color, 1);
}

float calc_attenuation(Attenuation atten, float dis){
	return atten.constant + atten.linear * dis + atten.expo * dis * dis;
}

vec3 calc_point_light_color(vec3 world_pos, vec3 normal, PointLight point_light){
	float dis = distance(world_pos, point_light.position);
	float atten = calc_attenuation(point_light.atten, dis);
	atten = max(1, atten);
	vec3 light_dir = world_pos - point_light.position;
	light_dir = normalize(light_dir);
	return calc_dir_color(light_dir, normal, point_light.base_light, 1, world_pos) / atten;
}

vec2 calc_tex_coord(){
	return gl_FragCoord.xy / g_win_size;
}

vec3 calc_dir_color(vec3 light_dir, vec3 normal, BaseLight base_light, float shadow_factor, vec3 world_pos){
	vec3 ambiant_color = base_light.color * base_light.ambiant_intensity;
	vec3 diffuse_color = vec3(0, 0, 0);
	vec3 specular_color = vec3(0, 0, 0);

	float diffuse_factor = dot(normal, -light_dir);
	if (diffuse_factor > 0){
		diffuse_color = base_light.color * base_light.diffuse_intensity;

		vec3 light_ref_dir = normalize(reflect(light_dir, normal));
		vec3 eye_dir = normalize(g_eye_w_pos - world_pos);
		float specular_factor = dot(eye_dir, light_ref_dir);
		if (specular_factor > 0){
			specular_factor = pow(specular_factor, g_specular_power);
			specular_color = base_light.color * g_mat_specular_intensity * specular_factor;
		}
	}

	return (ambiant_color + shadow_factor * (diffuse_color + specular_color));
}