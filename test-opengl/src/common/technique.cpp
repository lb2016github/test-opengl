#include "technique.h"
#include "technique.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "const.h"
#include "utils.h"

#define LOCATION_UNDEFINED 0xFFFFFFFF

Technique::Technique() : Technique("", "", "", "", "") {
}

Technique::Technique(const std::string& vertex_shader_path,
	const std::string& tsc_shader_path,
	const std::string& tse_shader_path,
	const std::string& geometry_shader_path,
	const std::string& fragment_shader_path
): m_program_id(0) {
	m_vertex_shader_path = vertex_shader_path;
	m_tc_shader_path = tsc_shader_path;
	m_te_shader_path = tse_shader_path;
	m_fragment_shader_path = fragment_shader_path;
	m_geometry_shader_path = geometry_shader_path;
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
	if (m_vertex_shader_path != "") {
		if (!add_shader(GL_VERTEX_SHADER, m_vertex_shader_path)) return false;
	}
	if (m_tc_shader_path != "") {
		if (!add_shader(GL_TESS_CONTROL_SHADER, m_tc_shader_path)) return false;
	}
	if (m_te_shader_path != "") {
		if (!add_shader(GL_TESS_EVALUATION_SHADER, m_te_shader_path)) return false;
	}
	if (m_geometry_shader_path != "") {
		if (!add_shader(GL_GEOMETRY_SHADER, m_geometry_shader_path)) return false;
	}
	if (m_fragment_shader_path != "") {
		if (!add_shader(GL_FRAGMENT_SHADER, m_fragment_shader_path)) return false;
	}
	if (!finalize()) {
		return false;
	}

	return true;
}

void Technique::enable() {
	glUseProgram(m_program_id);
}

bool Technique::add_shader(GLenum shader_type, const std::string& filename) {
	return add_shader(shader_type, filename.c_str());
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
DirectionLightTechnique::DirectionLightTechnique(){
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("direction_light.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("direction_light.frag");
}

bool DirectionLightTechnique::init() {

	if (!Technique::init()) return false;

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
	glUniform3f(m_color_location, m_dir_light.color[0], m_dir_light.color[1], m_dir_light.color[2]);
	glUniform1f(m_ambiance_intensity_location, m_dir_light.ambiance_intensity);
	glUniform1f(m_diffuse_intensity_location, m_dir_light.diffuse_intensity);
	glUniform3f(m_direction_location, m_dir_light.direction[0], m_dir_light.direction[1], m_dir_light.direction[2]);
}

void DirectionLightTechnique::set_transformation(Matrix& wvp, Matrix& w) {
	glUniformMatrix4fv(wvp_location, 1, false, wvp);
	glUniformMatrix4fv(w_locatioin, 1, false, w);
}

void DirectionLightTechnique::set_texture_unit(int unit_idx) {
	glUniform1i(m_sampler_location, unit_idx);
}

void DirectionLightTechnique::set_eye_pos(Vector3& eye_pos) {
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
PointLightTechnique::PointLightTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("point_light.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("point_light.frag");
}

bool PointLightTechnique::init() {

	if (!Technique::init()) return false;

	m_direction_light_location.init_location(
		m_program_id, "g_dir_light.base_light.color", "g_dir_light.base_light.ambiant_intensity", "g_dir_light.base_light.diffuse_intensity", "g_dir_light.direction"
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

	return true;
}

void PointLightTechnique::set_direction_light(DirectionLight& dir_light) {
	m_direction_light_location.set_light(dir_light);
}
void PointLightTechnique::set_point_lights(std::vector<PointLight>& point_light_list) {
	int num_light = point_light_list.size();
	glUniform1i(m_point_light_num_location, num_light);

	for (int i = 0; i < num_light; ++i){
		m_point_light_locations[i].set_light(point_light_list[i]);
	}
}
void PointLightTechnique::set_point_light(PointLight & point_light)
{
	glUniform1i(m_point_light_num_location, 1);
	m_point_light_locations[0].set_light(point_light);
}
void PointLightTechnique::set_eye_position(Vector3& eye_pos) {
	glUniform3f(m_eye_world_pos_location, eye_pos[0], eye_pos[1], eye_pos[2]);
}

void PointLightTechnique::set_specular_parameter(float spec_intensity, float spec_pow) {
	glUniform1f(m_mat_specular_intensity_location, spec_intensity);
	glUniform1f(m_specular_power_location, spec_pow);

}
void PointLightTechnique::set_transformation(Matrix& wvp, Matrix& world_trans) {
	glUniformMatrix4fv(m_wvp_location, 1, false, wvp);
	glUniformMatrix4fv(m_w_location, 1, false, world_trans);
}

void PointLightTechnique::set_texture_unit(int unit_idx) {
	glUniform1i(m_sampler_location, unit_idx);
}

/*********************************************************
Spot light technique
*********************************************************/
SpotLightTechnique::SpotLightTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("spot_light.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("spot_light.frag");
}

bool SpotLightTechnique::init() {
	bool rst = PointLightTechnique::init();

	for (int i = 0; i < MAX_SPOT_LIGHT_COUNT; ++i) {
		char name[128];
		snprintf(name, sizeof(name), "g_spot_light_list[%d].point_light.base_light.color", i);
		m_spot_light_locations[i].color = glGetUniformLocation(m_program_id, name);
		snprintf(name, sizeof(name), "g_spot_light_list[%d].point_light.base_light.ambiant_intensity", i);
		m_spot_light_locations[i].ambiant_intensity = glGetUniformLocation(m_program_id, name);
		snprintf(name, sizeof(name), "g_spot_light_list[%d].point_light.base_light.diffuse_intensity", i);
		m_spot_light_locations[i].diffuse_intensity = glGetUniformLocation(m_program_id, name);

		snprintf(name, sizeof(name), "g_spot_light_list[%d].point_light.position", i);
		m_spot_light_locations[i].position = glGetUniformLocation(m_program_id, name);

		snprintf(name, sizeof(name), "g_spot_light_list[%d].point_light.atten.constant", i);
		m_spot_light_locations[i].constant = glGetUniformLocation(m_program_id, name);
		snprintf(name, sizeof(name), "g_spot_light_list[%d].point_light.atten.linear", i);
		m_spot_light_locations[i].linear = glGetUniformLocation(m_program_id, name);
		snprintf(name, sizeof(name), "g_spot_light_list[%d].point_light.atten.expo", i);
		m_spot_light_locations[i].exp = glGetUniformLocation(m_program_id, name);

		snprintf(name, sizeof(name), "g_spot_light_list[%d].direction", i);
		m_spot_light_locations[i].direction = glGetUniformLocation(m_program_id, name);
		snprintf(name, sizeof(name), "g_spot_light_list[%d].cutoff", i);
		m_spot_light_locations[i].cutoff = glGetUniformLocation(m_program_id, name);
	}

	m_spot_light_num_location = glGetUniformLocation(m_program_id, "g_spot_light_num");

	return true;

}

void SpotLightTechnique::set_spot_lights(std::vector<SpotLight>& spot_light_list) {
	int light_num = spot_light_list.size();
	glUniform1i(m_spot_light_num_location, light_num);

	for (int i = 0; i < light_num; ++i) {
		m_spot_light_locations[i].set_light(spot_light_list[i]);
	}
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
	return is_valide();
}

void BaseLightLocation::set_light(BaseLight& light) {
	glUniform3f(color, light.color[0], light.color[1], light.color[2]);
	glUniform1f(ambiant_intensity, light.ambiance_intensity);
	glUniform1f(diffuse_intensity, light.diffuse_intensity);
}

bool BaseLightLocation::is_valide() {
	return color != LOCATION_UNDEFINED && ambiant_intensity != LOCATION_UNDEFINED && diffuse_intensity != LOCATION_UNDEFINED;
}

bool DirectionLightLocation::init_location(
	const GLuint program_id,
	const char* color_name, const char* ambiant_intensity_name, const char* diffuse_intensity_name, const char* direction_name
) {
	bool rst = BaseLightLocation::init_location(program_id, color_name, ambiant_intensity_name, diffuse_intensity_name);
	direction = glGetUniformLocation(program_id, direction_name);
	return rst && direction != LOCATION_UNDEFINED;
}

void DirectionLightLocation::set_light(DirectionLight& light) {
	BaseLightLocation::set_light(light);
	M3DVector3f norm_dir;
	m3dCopyVector3(norm_dir, light.direction);
	m3dNormalizeVector3(norm_dir);
	glUniform3f(direction, norm_dir[0], norm_dir[1], norm_dir[2]);
}

bool DirectionLightLocation::is_valide() {
	return BaseLightLocation::is_valide() && direction != LOCATION_UNDEFINED;
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

void PointLightLocation::set_light(PointLight& light) {
	BaseLightLocation::set_light(light);
	glUniform3f(position, light.position[0], light.position[1], light.position[2]);
	glUniform1f(constant, light.atten.constant);
	glUniform1f(linear, light.atten.linear);
	glUniform1f(exp, light.atten.exp);
}

bool PointLightLocation::is_valide() {
	return BaseLightLocation::is_valide() && position != LOCATION_UNDEFINED && constant != LOCATION_UNDEFINED && linear != LOCATION_UNDEFINED && exp != LOCATION_UNDEFINED;
}

void SpotLightLocation::set_light(SpotLight& light) {
	PointLightLocation::set_light(light);
	M3DVector3f norm;
	m3dCopyVector3(norm, light.direction);
	m3dNormalizeVector3(norm);

	glUniform3f(direction, norm[0], norm[1], norm[2]);
	glUniform1f(cutoff, cosf(m3dDegToRad(light.cutoff)));
}

/*********************************************************
ShadowMapTechnique
*********************************************************/

ShadowMapTechnique::ShadowMapTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("shadow_map.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("shadow_map.frag");
}

bool ShadowMapTechnique::init() {
	bool rst = SpotLightTechnique::init();

	m_light_wvp_location = glGetUniformLocation(m_program_id, "light_wvp");
	m_sampler_shadow_map_location = glGetUniformLocation(m_program_id, "g_sampler_shadow_map");

	if (m_light_wvp_location == INVALID_UNIFORM_LOCATION || m_light_wvp_location == INVALID_UNIFORM_LOCATION || !rst) return false;

	return true;
}

void ShadowMapTechnique::set_light_wvp_trans(Matrix& light_wvp) {
	glUniformMatrix4fv(m_light_wvp_location, 1, false, light_wvp);
}

void ShadowMapTechnique::set_shadow_map_tex_unit(unsigned int tex_idx) {
	glUniform1i(m_sampler_shadow_map_location, tex_idx);
}

/*********************************************************
ShadowMapTechnique
*********************************************************/
LightingTechnique::LightingTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("lighting_technique.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("lighting_technique.frag");
}

bool LightingTechnique::init() {
	bool rst = ShadowMapTechnique::init();
	m_sampler_normal_map_location = glGetUniformLocation(m_program_id, "g_sampler_normal_map");
	return rst && m_sampler_normal_map_location != INVALID_UNIFORM_LOCATION;
}

void LightingTechnique::set_normal_map_tex_unit(unsigned int texture_unit) {
	glUniform1i(m_sampler_normal_map_location, texture_unit);
}

/*********************************************************
Billboard Technique
*********************************************************/
BillboardTechnique::BillboardTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("billboard.vert");
	m_geometry_shader_path = PathManager::instance()->get_shader_dir("billboard.geom");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("billboard.frag");
}
bool BillboardTechnique::init() {
	if (!Technique::init()) return false;

	m_cam_pos_location = glGetUniformLocation(m_program_id, "cam_pos");
	m_width_location = glGetUniformLocation(m_program_id, "width");
	m_height_location = glGetUniformLocation(m_program_id, "height");
	m_color_map_location = glGetUniformLocation(m_program_id, "g_color_map");
	m_vp_location = glGetUniformLocation(m_program_id, "vp");
	return m_cam_pos_location != INVALID_UNIFORM_LOCATION && m_width_location != INVALID_UNIFORM_LOCATION &&
		m_height_location != INVALID_UNIFORM_LOCATION && m_vp_location != INVALID_UNIFORM_LOCATION &&
		m_color_map_location != INVALID_UNIFORM_LOCATION;
}

void BillboardTechnique::set_vp_trans(const Matrix& vp) {
	glUniformMatrix4fv(m_vp_location, 1, false, vp.data);
}

void BillboardTechnique::set_camera_position(const Vector3& cam_pos) {
	glUniform3f(m_cam_pos_location, cam_pos.data[0], cam_pos.data[1], cam_pos.data[2]);
}
void BillboardTechnique::set_billboard_size(float width, float height) {
	glUniform1f(m_width_location, width);
	glUniform1f(m_height_location, height);
}
void BillboardTechnique::set_color_map_tex_unit(unsigned int tex_unit) {
	glUniform1i(m_color_map_location, tex_unit);
}

/*********************************************************
Partical System Update Technique
*********************************************************/
PSUpdateTechnique::PSUpdateTechnique()
{
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("ps_update.vert");
	m_geometry_shader_path = PathManager::instance()->get_shader_dir("ps_update.geom");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("ps_update.frag");
}

PSUpdateTechnique::~PSUpdateTechnique()
{

}

bool PSUpdateTechnique::init() {
	m_program_id = glCreateProgram();
	if (m_program_id == 0) {
		fprintf(stderr, "Error creating shader program\n");
		return false;
	}
	if (m_vertex_shader_path != "") {
		if (!add_shader(GL_VERTEX_SHADER, m_vertex_shader_path)) return false;
	}
	if (m_geometry_shader_path != "") {
		if (!add_shader(GL_GEOMETRY_SHADER, m_geometry_shader_path)) return false;
	}
	if (m_fragment_shader_path != "") {
		if (!add_shader(GL_FRAGMENT_SHADER, m_fragment_shader_path)) return false;
	}

	const GLchar* varings[4];
	varings[0] = "gs_out.type";
	varings[1] = "gs_out.age";
	varings[2] = "gs_out.position";
	varings[3] = "gs_out.velocity";

	glTransformFeedbackVaryings(m_program_id, 4, varings, GL_INTERLEAVED_ATTRIBS);

	if (!finalize()) {
		return false;
	}

	m_time_location = glGetUniformLocation(m_program_id, "g_time");
	m_delta_time_location = glGetUniformLocation(m_program_id, "g_delta_time");
	m_lancher_life_time_location = glGetUniformLocation(m_program_id, "g_lancher_life_time");
	m_shell_life_time_location = glGetUniformLocation(m_program_id, "g_shell_life_time");
	m_secondary_shell_life_time_location = glGetUniformLocation(m_program_id, "g_secondary_shell_life_time");
	m_random_texture_location = glGetUniformLocation(m_program_id, "g_random_texture");

	if (m_time_location == INVALID_UNIFORM_LOCATION ||
		m_delta_time_location == INVALID_UNIFORM_LOCATION ||
		m_lancher_life_time_location == INVALID_UNIFORM_LOCATION ||
		m_shell_life_time_location == INVALID_UNIFORM_LOCATION ||
		m_secondary_shell_life_time_location == INVALID_UNIFORM_LOCATION ||
		m_random_texture_location == INVALID_UNIFORM_LOCATION
		) {
		return false;
	}
	return true;
}
void PSUpdateTechnique::set_time(float time) {
	glUniform1f(m_time_location, time);
}
void PSUpdateTechnique::set_delta_time(float delta_time) {
	glUniform1f(m_delta_time_location, delta_time);
}
void PSUpdateTechnique::set_life_time(float lancher_life_time, float shell_life_time, float secondary_shell_life_time) {
	glUniform1f(m_lancher_life_time_location, lancher_life_time);
	glUniform1f(m_shell_life_time_location, shell_life_time);
	glUniform1f(m_secondary_shell_life_time_location, secondary_shell_life_time);
}
void PSUpdateTechnique::set_random_texture_unit(unsigned int texture_unit) {
	glUniform1i(m_random_texture_location, texture_unit);
}


/*********************************************************
Picking Technique
*********************************************************/
PickingTechnique::PickingTechnique()
{
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("picking.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("picking.frag");
}

PickingTechnique::~PickingTechnique()
{
}

bool PickingTechnique::init() {
	if (!Technique::init()) return false;

	m_obj_id_location = glGetUniformLocation(m_program_id, "g_obj_id");
	m_mesh_id_location = glGetUniformLocation(m_program_id, "g_mesh_id");
	m_wvp_location = glGetUniformLocation(m_program_id, "mvp");

	return m_obj_id_location != INVALID_UNIFORM_LOCATION && m_mesh_id_location != INVALID_UNIFORM_LOCATION && m_wvp_location != INVALID_UNIFORM_LOCATION;
}
void PickingTechnique::set_obj_id(unsigned int obj_id) {
	glUniform1i(m_obj_id_location, obj_id);
}
void PickingTechnique::set_mesh_id(unsigned int mesh_id) {
	glUniform1i(m_mesh_id_location, mesh_id);
}
void PickingTechnique::set_wvp(const Matrix& wvp) {
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp.data);
}

void PickingTechnique::on_draw_start_callback(unsigned int mesh_id) {
	set_mesh_id(mesh_id);
}

/*********************************************************
Simple Color Technique
*********************************************************/
SimpleColorTechnique::SimpleColorTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("simple_color.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("simple_color.frag");
}
SimpleColorTechnique::~SimpleColorTechnique() {

}

bool SimpleColorTechnique::init() {
	if (!Technique::init()) return false;

	m_wvp_location = glGetUniformLocation(m_program_id, "wvp");

	return m_wvp_location != INVALID_UNIFORM_LOCATION;
	
}
void SimpleColorTechnique::set_wvp(const Matrix& wvp) {
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp.data);
}

/*********************************************************
Simple Show Technique
*********************************************************/

SimpleShowTechnique::SimpleShowTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("simple_show.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("simple_show.frag");
}
SimpleShowTechnique::~SimpleShowTechnique() {

}

bool SimpleShowTechnique::init() {
	if (!Technique::init()) return false;

	m_wvp_location = glGetUniformLocation(m_program_id, "wvp");
	m_color_sampler_location = glGetUniformLocation(m_program_id, "g_color_sampler");

	return m_wvp_location != INVALID_UNIFORM_LOCATION && m_color_sampler_location != INVALID_UNIFORM_LOCATION;
}
void SimpleShowTechnique::set_wvp(const Matrix& wvp) {
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp.data);
}
void SimpleShowTechnique::set_tex_index(unsigned int tex_index) {
	glUniform1i(m_color_sampler_location, tex_index);
}

/*********************************************************
Tessellation Technique
*********************************************************/
TessellationTechnique::TessellationTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("tessellation.vert");
	m_tc_shader_path = PathManager::instance()->get_shader_dir("tessellation.tessc");
	m_te_shader_path = PathManager::instance()->get_shader_dir("tessellation.tesse");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("tessellation.frag");
}
TessellationTechnique::~TessellationTechnique() {

}

bool TessellationTechnique::init() {
	if (!Technique::init()) return false;

	m_world_location = glGetUniformLocation(m_program_id, "g_world");
	m_tess_level_location = glGetUniformLocation(m_program_id, "g_tess_level");
	m_vp_location = glGetUniformLocation(m_program_id, "g_vp");
	m_sampler_color_location = glGetUniformLocation(m_program_id, "g_sampler_color_map");
	bool rst = m_dir_light_location.init_location(m_program_id, "dir_light.color", "dir_light.ambiant_intensity", "dir_light.diffuse_intensity", "dir_light.direction");
	m_eye_pos = glGetUniformLocation(m_program_id, "g_eye_pos");
	m_specular_intensity = glGetUniformLocation(m_program_id, "materia_specular_intensity");
	m_specular_power = glGetUniformLocation(m_program_id, "specular_power");

	if (
		m_world_location == INVALID_UNIFORM_LOCATION ||
		m_tess_level_location == INVALID_UNIFORM_LOCATION ||
		m_vp_location == INVALID_UNIFORM_LOCATION ||
		m_sampler_color_location == INVALID_UNIFORM_LOCATION ||
		m_eye_pos == INVALID_UNIFORM_LOCATION ||
		m_specular_intensity == INVALID_UNIFORM_LOCATION ||
		m_specular_power == INVALID_UNIFORM_LOCATION ||
		!rst
		) {
		return false;
	}
	
	return true;

}
void TessellationTechnique::set_world(const Matrix& w) {
	glUniformMatrix4fv(m_world_location, 1, false, w.data);
}
void TessellationTechnique::set_vp(const Matrix& vp) {
	glUniformMatrix4fv(m_vp_location, 1, false, vp.data);
}
void TessellationTechnique::set_tex_color_index(unsigned int color_index) {
	glUniform1i(m_sampler_color_location, color_index);
}
void TessellationTechnique::set_tess_level(float tess_level) {
	glUniform1f(m_tess_level_location, tess_level);
}

void TessellationTechnique::set_dir_light(DirectionLight& dir_light) {
	m_dir_light_location.set_light(dir_light);
}
void TessellationTechnique::set_eye_pos(const Vector3& eye_pos) {
	glUniform3f(m_eye_pos, eye_pos.data[0], eye_pos.data[1], eye_pos.data[2]);
}
void TessellationTechnique::set_specular_param(float intensity, float power) {
	glUniform1f(m_specular_intensity, intensity);
	glUniform1f(m_specular_power, power);
}


/*********************************************************
Instanced Rending Technique
*********************************************************/
InstanceRenderingTechnique::InstanceRenderingTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("instance_render.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("instance_render.frag");
}
InstanceRenderingTechnique::~InstanceRenderingTechnique() {

}

bool InstanceRenderingTechnique::init()
{
	bool rst = Technique::init();
	m_sampler_color_location = glGetUniformLocation(m_program_id, "g_sampler_color");
	return rst && m_sampler_color_location != INVALID_UNIFORM_LOCATION;

}
void InstanceRenderingTechnique::set_tex_color_index(unsigned int color_index)
{
	glUniform1i(m_sampler_color_location, color_index);
}

/*********************************************************
Deffered Rending Technique
*********************************************************/
DSGeometryTechnique::DSGeometryTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("ds_geometry_pass.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("ds_geometry_pass.frag");
}
DSGeometryTechnique::~DSGeometryTechnique() {
	
}
bool DSGeometryTechnique::init() {
	bool rst = Technique::init();
	m_color_sampler_location = glGetUniformLocation(m_program_id, "g_color_sampler");
	m_wvp_location = glGetUniformLocation(m_program_id, "wvp");
	m_world_location = glGetUniformLocation(m_program_id, "world");
	return rst && m_color_sampler_location != INVALID_UNIFORM_LOCATION &&
		m_wvp_location != INVALID_UNIFORM_LOCATION &&
		m_world_location != INVALID_UNIFORM_LOCATION;
}
void DSGeometryTechnique::set_tex_color_index(unsigned int color_index) {
	glUniform1i(m_color_sampler_location, color_index);
}
void DSGeometryTechnique::set_wvp_trans(const Matrix& wvp) {
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp.data);
}
void DSGeometryTechnique::set_world_trans(const Matrix& world) {
	glUniformMatrix4fv(m_world_location, 1, GL_FALSE, world.data);
}

bool DSLightTechnique::init() {
	bool rst1 = Technique::init();
	m_wvp_location = glGetUniformLocation(m_program_id, "wvp");
	m_diffuse_sampler_location = glGetUniformLocation(m_program_id, "g_diffuse_sampler");
	m_position_sampler_location = glGetUniformLocation(m_program_id, "g_position_sampler");
	m_normal_sampler_location = glGetUniformLocation(m_program_id, "g_normal_sampler");
	m_win_size_location = glGetUniformLocation(m_program_id, "g_win_size");
	m_eye_pos_location = glGetUniformLocation(m_program_id, "g_eye_w_pos");
	m_specular_pow_location = glGetUniformLocation(m_program_id, "g_specular_power");
	m_specular_intensity_location = glGetUniformLocation(m_program_id, "g_mat_specular_intensity");
	
	std::vector<GLuint> locations = {
		m_wvp_location, m_diffuse_sampler_location, m_position_sampler_location, m_normal_sampler_location,
		m_win_size_location, m_eye_pos_location, m_specular_pow_location, m_specular_intensity_location
	};

	if (!rst1) {
		return false;
	}
	for (std::vector<GLuint>::iterator iter = locations.begin(); iter != locations.end(); ++iter) {
		if (*iter == INVALID_UNIFORM_LOCATION) return false;
	}
	return true;

}
void DSLightTechnique::set_wvp_trans(const Matrix& wvp) {
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp.data);
}
void DSLightTechnique::set_diffuse_sampler_index(unsigned int tex_index) {
	glUniform1i(m_diffuse_sampler_location, tex_index);
}
void DSLightTechnique::set_position_sampler_index(unsigned int tex_index) {
	glUniform1i(m_position_sampler_location, tex_index);
}
void DSLightTechnique::set_normal_sampler_index(unsigned int tex_index) {
	glUniform1i(m_normal_sampler_location, tex_index);
}
void DSLightTechnique::set_window_size(int win_width, int win_height) {
	glUniform2f(m_win_size_location, win_width, win_height);
}
void DSLightTechnique::set_eye_pos(const Vector3 w_pos) {
	glUniform3f(m_eye_pos_location, w_pos.data[0], w_pos.data[1], w_pos.data[2]);
}
void DSLightTechnique::set_specular_param(float pow, float mat_specular) {
	glUniform1f(m_specular_pow_location, pow);
	glUniform1f(m_specular_intensity_location, mat_specular);
}

DSDirLightTechnique::DSDirLightTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("ds_light_pass.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("ds_dir_light_pass.frag");
}
DSDirLightTechnique::~DSDirLightTechnique() {

}
bool DSDirLightTechnique::init() {
	bool rst = DSLightTechnique::init();
	bool rst2 = m_dir_light_location.init_location(m_program_id,
		"g_dir_light.base_light.color", "g_dir_light.base_light.ambiant_intensity", "g_dir_light.base_light.diffuse_intensity", "g_dir_light.direction"
	);
	return rst && rst2;
}

void DSDirLightTechnique::set_dir_light(DirectionLight& dir_light) {
	m_dir_light_location.set_light(dir_light);
}


DSPointLightTechnique::DSPointLightTechnique() {
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("ds_light_pass.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("ds_point_light_pass.frag");
}
DSPointLightTechnique::~DSPointLightTechnique() {

}
bool DSPointLightTechnique::init() {
	bool rst = DSLightTechnique::init();
	char tmp_str[128];
	for (int i = 0; i < MAX_POINT_LIGHT_COUNT; ++i) {
		sprintf(tmp_str, "g_point_light_list[%d].base_light.color", i);
		m_light_locations[i].color = glGetUniformLocation(m_program_id, tmp_str);
		sprintf(tmp_str, "g_point_light_list[%d].base_light.ambiant_intensity", i);
		m_light_locations[i].ambiant_intensity = glGetUniformLocation(m_program_id, tmp_str);
		sprintf(tmp_str, "g_point_light_list[%d].base_light.diffuse_intensity", i);
		m_light_locations[i].diffuse_intensity = glGetUniformLocation(m_program_id, tmp_str);
		sprintf(tmp_str, "g_point_light_list[%d].position", i);
		m_light_locations[i].position = glGetUniformLocation(m_program_id, tmp_str);
		sprintf(tmp_str, "g_point_light_list[%d].atten.constant", i);
		m_light_locations[i].constant = glGetUniformLocation(m_program_id, tmp_str);
		sprintf(tmp_str, "g_point_light_list[%d].atten.linear", i);
		m_light_locations[i].linear = glGetUniformLocation(m_program_id, tmp_str);
		sprintf(tmp_str, "g_point_light_list[%d].atten.expo", i);
		m_light_locations[i].exp = glGetUniformLocation(m_program_id, tmp_str);
	}
	m_light_num_location = glGetUniformLocation(m_program_id, "g_point_light_num");
	for (int i = 0; i < MAX_POINT_LIGHT_COUNT; ++i) {
		if (!m_light_locations[i].is_valide()) return false;
	}
	return m_light_num_location != INVALID_UNIFORM_LOCATION;
}
void DSPointLightTechnique::set_point_lights(int light_num, PointLight lights[], int start_index) {
	if (light_num > MAX_POINT_LIGHT_COUNT) light_num = MAX_POINT_LIGHT_COUNT;
	glUniform1i(m_light_num_location, light_num);
	for (int i = 0; i < light_num; ++i) {
		m_light_locations[i].set_light(lights[start_index + i]);
	}
}

SilhouetteDetectionTechnique::SilhouetteDetectionTechnique()
{
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("silhouette.vert");
	m_geometry_shader_path = PathManager::instance()->get_shader_dir("silhouette.geom");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("silhouette.frag");
}

SilhouetteDetectionTechnique::~SilhouetteDetectionTechnique()
{
}

bool SilhouetteDetectionTechnique::init()
{
	bool rst = Technique::init();
	m_wvp_location = glGetUniformLocation(m_program_id, "g_wvp");
	m_world_location = glGetUniformLocation(m_program_id, "g_world_mat");
	m_light_pos_location = glGetUniformLocation(m_program_id, "g_light_pos");
	return rst && m_wvp_location != INVALID_UNIFORM_LOCATION && m_world_location != INVALID_UNIFORM_LOCATION && m_light_pos_location != INVALID_UNIFORM_LOCATION;
}

void SilhouetteDetectionTechnique::set_wvp_mat(Matrix & wvp)
{
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp.data);
}

void SilhouetteDetectionTechnique::set_world_mat(Matrix & world_mat)
{
	glUniformMatrix4fv(m_world_location, 1, GL_FALSE, world_mat.data);
}

void SilhouetteDetectionTechnique::set_light_pos(Vector3 & world_pos)
{
	glUniform3f(m_light_pos_location, world_pos[0], world_pos[1], world_pos[2]);
}

NullTechnique::NullTechnique()
{
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("null_technique.vert");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("null_technique.frag");
}

NullTechnique::~NullTechnique()
{
}

bool NullTechnique::init()
{
	bool rst = Technique::init();
	m_wvp_location = glGetUniformLocation(m_program_id, "g_wvp");
	return rst && m_wvp_location != INVALID_UNIFORM_LOCATION;
}

void NullTechnique::set_wvp_mtx(const Matrix & wvp)
{
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp.data);
}

StencilShadowVolumeTechnique::StencilShadowVolumeTechnique()
{
	m_vertex_shader_path = PathManager::instance()->get_shader_dir("stencil_shadow_volume.vert");
	m_geometry_shader_path = PathManager::instance()->get_shader_dir("stencil_shadow_volume.geom");
	m_fragment_shader_path = PathManager::instance()->get_shader_dir("stencil_shadow_volume.frag");
}

StencilShadowVolumeTechnique::~StencilShadowVolumeTechnique()
{
}

bool StencilShadowVolumeTechnique::init()
{
	bool rst = Technique::init();
	m_world_trans_location = glGetUniformLocation(m_program_id, "g_world_trans");
	m_light_position_location = glGetUniformLocation(m_program_id, "g_light_position");
	m_vp_location = glGetUniformLocation(m_program_id, "g_vp");
	return rst && m_world_trans_location != INVALID_UNIFORM_LOCATION && m_light_position_location != INVALID_UNIFORM_LOCATION && m_vp_location != INVALID_UNIFORM_LOCATION;
}

void StencilShadowVolumeTechnique::set_world_mtx(const Matrix & world_trans)
{
	glUniformMatrix4fv(m_world_trans_location, 1, GL_FALSE, world_trans.data);
}

void StencilShadowVolumeTechnique::set_light_position(const Vector3 & light_pos)
{
	glUniform3f(m_light_position_location, light_pos.data[0], light_pos.data[1], light_pos.data[2]);
}

void StencilShadowVolumeTechnique::set_vp_mtx(const Matrix & vp_mtx)
{
	glUniformMatrix4fv(m_vp_location, 1, GL_FALSE, vp_mtx.data);
}
