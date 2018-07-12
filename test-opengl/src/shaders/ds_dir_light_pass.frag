#version 330

struct BaseLight{
	vec3 color;
	float ambiant_intensity;
	float diffuse_intensity;
};

struct DirectionLight{
	BaseLight base_light;
	vec3 direction;
};

out vec4 frag_color;

uniform sampler2D g_diffuse_sampler;
uniform sampler2D g_position_sampler;
uniform sampler2D g_normal_sampler;

uniform DirectionLight g_dir_light;

uniform vec2 g_win_size;
uniform vec3 g_eye_w_pos;
uniform float g_specular_power;
uniform float g_mat_specular_intensity;


vec2 calc_tex_coord();
vec3 calc_dir_color(vec3 light_dir, vec3 normal, BaseLight base_light, float shadow_factor, vec3 world_pos);

void main(){
	vec2 tex_coord = calc_tex_coord();
	vec3 diffuse_color = texture(g_diffuse_sampler, tex_coord).xyz;
	vec3 positon = texture(g_position_sampler, tex_coord).xyz;
	vec3 normal = texture(g_normal_sampler, tex_coord).xyz;
	frag_color.xyz = diffuse_color * calc_dir_color(g_dir_light.direction, normal, g_dir_light.base_light, 1, positon);
	frag_color.w = 1;
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

