#include "technique.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#define LOCATION_UNDEFINED 0xFFFFFFFF

Technique::Technique() : m_program_id(0){
}

Technique::~Technique() {
	for (ShaderObjList::iterator it = m_shader_obj_list.begin(); it != m_shader_obj_list.end(); ++it) {
		glDeleteShader(*it);
	}

	if (m_program_id != 0) {
		glDeleteProgram(m_program_id);
		m_program_id = 0;
	}
}

bool Technique::init() {
	m_program_id = glCreateProgram();
	if (m_program_id == 0) {
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}
	return true;
}

void Technique::enable() {
	glUseProgram(m_program_id);
}

bool Technique::add_shader(GLenum shader_type, const char* filename) {
	// create shader
	GLuint shader_id = glCreateShader(shader_type);
	// load shader data
	std::ifstream fs(filename, std::ios::in);
	std::string shader_code;
	if (fs.is_open()) {
		std::stringstream ss;
		ss << fs.rdbuf();
		shader_code = ss.str();
		fs.close();
	}
	else {
		fprintf(stderr, "Error reading shader file %s", filename);
		return false;
	}
	// compile shader
	const char* code_pointer = shader_code.c_str();
	glShaderSource(shader_id, 1, &code_pointer, NULL);
	glCompileShader(shader_id);
	// check is valid
	GLint result = GL_FALSE;
	int info_log_length = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0 ) {
		std::vector<char> error_msg(info_log_length);
		glGetShaderInfoLog(shader_id, info_log_length, NULL, &error_msg[0]);
		printf("%s\n", &error_msg[0]);
	}
	if (result == GL_FALSE) return false;

	// add shader obj
	m_shader_obj_list.push_back(shader_id);
	return true;
}

bool Technique::finalize() {
	// attach shader
	for (std::list<GLuint>::iterator it = m_shader_obj_list.begin(); it != m_shader_obj_list.end(); ++it) {
		glAttachShader(m_program_id, *it);
	}
	// link
	glLinkProgram(m_program_id);
	// check is valid
	GLint result = GL_FALSE;
	int info_log_length = 0;
	glGetProgramiv(m_program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0) {
		std::vector<char> msg(info_log_length);
		glGetProgramInfoLog(m_program_id, info_log_length, NULL, &msg[0]);
		printf("%s\n", &msg[0]);
	}
	if (result == GL_FALSE) return false;
	else return true;
}

////////////////////////////////////DirectionLightTechnique////////////////
bool DirectionLightTechnique::init() {
	if (!Technique::init()) return false;
	// add shader
	if (!add_shader(GL_VERTEX_SHADER, "shaders/direction_light.vert")) return false;
	if (!add_shader(GL_FRAGMENT_SHADER, "shaders/direction_light.frag")) return false;
	// finalize
	if (!finalize()) return false;

	wvp_location = glGetUniformLocation(m_program_id, "wvp");
	w_locatioin = glGetUniformLocation(m_program_id, "w");
	m_color_location = glGetUniformLocation(m_program_id, "dir_light.color");
	m_ambiance_intensity_location = glGetUniformLocation(m_program_id, "dir_light.ambiant_intensity");
	m_diffuse_intensity_location = glGetUniformLocation(m_program_id, "dir_light.diffuse_intensity");
	m_direction_location = glGetUniformLocation(m_program_id, "dir_light.direction");

	m_sampler_location = glGetUniformLocation(m_program_id, "g_sampler");

	m_eye_pos_location = glGetUniformLocation(m_program_id, "eye_pos");
	m_materia_specular_intensity_location = glGetUniformLocation(m_program_id, "materia_specular_intensity");

	m_specular_power = glGetUniformLocation(m_program_id, "specular_power");

#define UNDEFINED_LOCATION 0xFFFFFFFF
	if (wvp_location == UNDEFINED_LOCATION ||
		w_locatioin == UNDEFINED_LOCATION ||
		m_color_location == UNDEFINED_LOCATION ||
		m_ambiance_intensity_location == UNDEFINED_LOCATION ||
		m_diffuse_intensity_location == UNDEFINED_LOCATION ||
		m_direction_location == UNDEFINED_LOCATION ||
		m_sampler_location == UNDEFINED_LOCATION ||
		m_eye_pos_location == UNDEFINED_LOCATION ||
		m_materia_specular_intensity_location == UNDEFINED_LOCATION
		){
		printf("Error: get locations\n");
		return false;
	}
#undef UNDEFINED_LOCATION

	return true;
}

void DirectionLightTechnique::set_light(DirectionLight& m_dir_light) {
	glUniform3f(m_color_location, m_dir_light.m_color[0], m_dir_light.m_color[1], m_dir_light.m_color[2]);
	glUniform1f(m_ambiance_intensity_location, m_dir_light.m_ambiance_intensity);
	glUniform1f(m_diffuse_intensity_location, m_dir_light.m_diffuse_intensity);
	glUniform3f(m_direction_location, m_dir_light.m_direction[0], m_dir_light.m_direction[1], m_dir_light.m_direction[2]);
}

void DirectionLightTechnique::set_transformation(M3DMatrix44f wvp, M3DMatrix44f w) {
	glUniformMatrix4fv(wvp_location, 1, false, wvp);
	glUniformMatrix4fv(w_locatioin, 1, false, w);
}

void DirectionLightTechnique::set_texture_unit(int unit_idx) {
	glUniform1i(m_sampler_location, unit_idx);
}

void DirectionLightTechnique::set_eye_pos(M3DVector3f eye_pos) {
	glUniform3f(m_eye_pos_location, eye_pos[0], eye_pos[1], eye_pos[2]);
}

void DirectionLightTechnique::set_material_specular_intensity(float materia_specular_intensity) {
	glUniform1f(m_materia_specular_intensity_location, materia_specular_intensity);
}

void DirectionLightTechnique::set_specular_power(float power) {
	glUniform1f(m_specular_power, power);
}

/*************************************
Point Light Technique
*****************************************/
bool PointLightTechnique::init() {
	if (!Technique::init()) return false;
	if (!add_shader(GL_VERTEX_SHADER, "shaders/point_light.vert")) return false;
	if (!add_shader(GL_FRAGMENT_SHADER, "shaders/point_light.frag")) return false;
	if (!finalize()) return false;

	m_direction_light_location.init_location(
		m_program_id, "g_dir_light.base_light.color", "g_dir_light.base_light.ambiant_intensity", "g_dir_light.base_light.ambiant_intensity", "g_dir_light.direction"
	);

	for (int i = 0; i < MAX_POINT_LIGHT_COUNT; ++i) {
		char name[128];
		snprintf(name, sizeof(name), "g_point_light_list[%d].base_light.color", i);
		m_point_light_locations[i].color = glGetUniformLocation(m_program_id, name);
		snprintf(name, sizeof(name), "g_point_light_list[%d].base_light.ambiant_intensity", i);
		m_point_light_locations[i].ambiant_intensity = glGetUniformLocation(m_program_id, name);
		snprintf(name, sizeof(name), "g_point_light_list[%d].base_light.diffuse_intensity", i);
		m_point_light_locations[i].diffuse_intensity = glGetUniformLocation(m_program_id, name);

		snprintf(name, sizeof(name), "g_point_light_list[%d].position", i);
		m_point_light_locations[i].position = glGetUniformLocation(m_program_id, name);

		snprintf(name, sizeof(name), "g_point_light_list[%d].atten.constant", i);
		m_point_light_locations[i].constant = glGetUniformLocation(m_program_id, name);
		snprintf(name, sizeof(name), "g_point_light_list[%d].atten.linear", i);
		m_point_light_locations[i].linear = glGetUniformLocation(m_program_id, name);
		snprintf(name, sizeof(name), "g_point_light_list[%d].atten.expo", i);
		m_point_light_locations[i].exp = glGetUniformLocation(m_program_id, name);
	}

	m_point_light_num_location = glGetUniformLocation(m_program_id, "g_point_light_num");
	m_eye_world_pos_location = glGetUniformLocation(m_program_id, "g_eye_w_pos");
	m_sampler_location = glGetUniformLocation(m_program_id, "g_sampler");
	m_mat_specular_intensity_location = glGetUniformLocation(m_program_id, "g_mat_specular_intensity");
	m_specular_power_location = glGetUniformLocation(m_program_id, "g_specular_power");
	m_wvp_location = glGetUniformLocation(m_program_id, "wvp");
	m_w_location = glGetUniformLocation(m_program_id, "w");
}

void PointLightTechnique::set_direction_light(DirectionLight& dir_light) {
	m_direction_light_location.set_light(dir_light.m_color, dir_light.m_ambiance_intensity, dir_light.m_diffuse_intensity, dir_light.m_direction);
}
void PointLightTechnique::set_point_lights(std::vector<PointLight>& point_light_list) {
	int num_light = point_light_list.size();
	glUniform1i(m_point_light_num_location, num_light);

	for (int i = 0; i < num_light; ++i){
		m_point_light_locations[i].set_light(
			point_light_list[i].m_color, point_light_list[i].m_ambiance_intensity, point_light_list[i].m_diffuse_intensity,
			point_light_list[i].position, point_light_list[i].atten.constant, point_light_list[i].atten.linear, point_light_list[i].atten.exp
		);
	}
}
void PointLightTechnique::set_eye_position(M3DVector3f eye_pos) {
	glUniform3f(m_eye_world_pos_location, eye_pos[0], eye_pos[1], eye_pos[2]);
}

void PointLightTechnique::set_specular_parameter(float spec_intensity, float spec_pow) {
	glUniform1f(m_mat_specular_intensity_location, spec_intensity);
	glUniform1f(m_specular_power_location, spec_pow);

}
void PointLightTechnique::set_transformation(M3DMatrix44f wvp, M3DMatrix44f world_trans) {
	glUniformMatrix4fv(m_wvp_location, 1, false, wvp);
	glUniformMatrix4fv(m_w_location, 1, false, world_trans);
}

void PointLightTechnique::set_texture_unit(int unit_idx) {
	glUniform1f(m_sampler_location, unit_idx);
}


/*********************************************************
light location
*********************************************************/
bool BaseLightLocation::init_location(
	const GLuint program_id,
	const char* color_name, const char* ambiant_intensity_name, const char* diffuse_intensity_name
) {
	color = glGetUniformLocation(program_id, color_name);
	ambiant_intensity = glGetUniformLocation(program_id, ambiant_intensity_name);
	diffuse_intensity = glGetUniformLocation(program_id, diffuse_intensity_name);
	return color != LOCATION_UNDEFINED && ambiant_intensity != LOCATION_UNDEFINED && diffuse_intensity != LOCATION_UNDEFINED;
}

void BaseLightLocation::set_light(M3DVector3f color0, float ambiant_intensity0, float diffuse_intensity0) {
	glUniform3f(color, color0[0], color0[1], color0[2]);
	glUniform1f(ambiant_intensity, ambiant_intensity0);
	glUniform1f(diffuse_intensity, diffuse_intensity0);
}

bool DirectionLightLocation::init_location(
	const GLuint program_id,
	const char* color_name, const char* ambiant_intensity_name, const char* diffuse_intensity_name, const char* direction_name
) {
	bool rst = BaseLightLocation::init_location(program_id, color_name, ambiant_intensity_name, diffuse_intensity_name);
	direction = glGetUniformLocation(program_id, direction_name);
	return rst && direction != LOCATION_UNDEFINED;
}

void DirectionLightLocation::set_light(M3DVector3f color0, float ambiant_intensity0, float diffuse_intensity0, M3DVector3f direction0) {
	BaseLightLocation::set_light(color0, ambiant_intensity0, diffuse_intensity0);
	M3DVector3f norm_dir;
	m3dCopyVector3(norm_dir, direction0);
	m3dNormalizeVector3(norm_dir);
	glUniform3f(direction, norm_dir[0], norm_dir[1], norm_dir[2]);
}

bool PointLightLocation::init_location(
	const GLuint program_id,
	const char* color_name, const char* ambiant_intensity_name, const char* diffuse_intensity_name,
	const char* position_name, const char* constant_name, const char* linear_name, const char* exp_name
) {
	bool rst = BaseLightLocation::init_location(program_id, color_name, ambiant_intensity_name, diffuse_intensity_name);
	position = glGetUniformLocation(program_id, position_name);
	constant = glGetUniformLocation(program_id, constant_name);
	linear = glGetUniformLocation(program_id, linear_name);
	exp = glGetUniformLocation(program_id, exp_name);
	return rst && position != LOCATION_UNDEFINED && constant != LOCATION_UNDEFINED && linear != LOCATION_UNDEFINED && exp != LOCATION_UNDEFINED;
}

void PointLightLocation::set_light(
	M3DVector3f color0, float ambiant_intensity0, float diffuse_intensity0,
	M3DVector3f position0, float constant0, float linear0, float exp0
) {
	BaseLightLocation::set_light(color0, ambiant_intensity0, diffuse_intensity0);
	glUniform3f(position, position0[0], position0[1], position0[2]);
	glUniform1f(constant, constant0);
	glUniform1f(linear, linear0);
	glUniform1f(exp, exp0);
}