#include "technique.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


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
	if (!add_shader(GL_VERTEX_SHADER, "shaders/ambiance.vert")) return false;
	if (!add_shader(GL_FRAGMENT_SHADER, "shaders/ambiance.frag")) return false;
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

