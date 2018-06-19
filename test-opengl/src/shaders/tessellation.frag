#version 410 core

struct TESOut{
	vec2 tex_coord;
	vec3 w_normal;
	vec3 w_pos;
};

struct DirectionalLight{
	vec3 color;
	float ambiant_intensity;
	float diffuse_intensity;
	vec3 direction;
};

in TESOut tes_out;

out vec4 frag_color;

uniform sampler2D g_sampler_color_map;
uniform DirectionalLight dir_light;
uniform vec3 g_eye_pos;
uniform float materia_specular_intensity;
uniform float specular_power;

void main(){
	float diffuse_factor = 0;
	float ambiant_factor = dir_light.ambiant_intensity;
	float specular_factor = 0;

	vec3 normal_1 = normalize(tes_out.w_normal);

	diffuse_factor = dot(normal_1, -dir_light.direction);
	
	if (diffuse_factor > 0){
		diffuse_factor = diffuse_factor * dir_light.diffuse_intensity;

		vec3 ref_dir, eye_dir;
		ref_dir = normalize(reflect(dir_light.direction, normal_1));
		eye_dir = normalize(g_eye_pos - tes_out.w_pos);
		specular_factor = dot(ref_dir, eye_dir);

		if (specular_factor > 0){
			specular_factor = pow(specular_factor, specular_power);
			specular_factor = specular_factor * materia_specular_intensity;
		}else{
			specular_factor = 0;
		}
	}
	else{
		diffuse_factor = 0;
	}

	vec3 color = dir_light.color * (diffuse_factor + ambiant_factor + specular_factor);
	frag_color = texture(g_sampler_color_map, tes_out.tex_coord) * vec4(color, 1);

}