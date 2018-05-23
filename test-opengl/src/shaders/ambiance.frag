#version 330 core
in vec2 o_tex_coor0;
in vec3 o_normal;
in vec3 o_w_pos;
out vec4 out_color;

struct DirectionalLight{
	vec3 color;
	float ambiant_intensity;
	float diffuse_intensity;
	vec3 direction;
};

uniform sampler2D g_sampler;
uniform DirectionalLight dir_light;
uniform vec3 eye_pos;
uniform float materia_specular_intensity;
uniform float specular_power;

void main(){
	float diffuse_factor = 0;
	float ambiant_factor = dir_light.ambiant_intensity;
	float specular_factor = 0;

	vec3 normal_1 = normalize(o_normal);

	diffuse_factor = dot(normal_1, -dir_light.direction);
	
	if (diffuse_factor > 0){
		diffuse_factor = diffuse_factor * dir_light.diffuse_intensity;

		vec3 ref_dir, eye_dir;
		ref_dir = normalize(reflect(dir_light.direction, normal_1));
		eye_dir = normalize(eye_pos - o_w_pos);
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
	out_color = texture2D(g_sampler, o_tex_coor0.xy) * vec4(color, 1);

}