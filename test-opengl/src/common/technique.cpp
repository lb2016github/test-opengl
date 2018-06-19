#include "technique.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "const.h"

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
	m_vertex_shader_path = "shaders/direction_light.vert";
	m_fragment_shader_path = "shaders/direction_light.frag";
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
PointLightTechnique::PointLightTechnique() {
	m_vertex_shader_path = "shaders/point_light.vert";
	m_fragment_shader_path = "shaders/point_light.frag";
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
Spot light technique
*********************************************************/
SpotLightTechnique::SpotLightTechnique() {
	m_vertex_shader_path = "shaders/spot_light.vert";
	m_fragment_shader_path = "shaders/spot_light.frag";
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
	return color != LOCATION_UNDEFINED && ambiant_intensity != LOCATION_UNDEFINED && diffuse_intensity != LOCATION_UNDEFINED;
}

void BaseLightLocation::set_light(BaseLight& light) {
	glUniform3f(color, light.color[0], light.color[1], light.color[2]);
	glUniform1f(ambiant_intensity, light.ambiance_intensity);
	glUniform1f(diffuse_intensity, light.diffuse_intensity);
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
	m_vertex_shader_path = "shaders/shadow_map.vert";
	m_fragment_shader_path = "shaders/shadow_map.frag";
}

bool ShadowMapTechnique::init() {
	bool rst = SpotLightTechnique::init();

	m_light_wvp_location = glGetUniformLocation(m_program_id, "light_wvp");
	m_sampler_shadow_map_location = glGetUniformLocation(m_program_id, "g_sampler_shadow_map");

	if (m_light_wvp_location == INVALID_UNIFORM_LOCATION || m_light_wvp_location == INVALID_UNIFORM_LOCATION || !rst) return false;

	return true;
}

void ShadowMapTechnique::set_light_wvp_trans(M3DMatrix44f light_wvp) {
	glUniformMatrix4fv(m_light_wvp_location, 1, false, light_wvp);
}

void ShadowMapTechnique::set_shadow_map_tex_unit(unsigned int tex_idx) {
	glUniform1i(m_sampler_shadow_map_location, tex_idx);
}

/*********************************************************
ShadowMapTechnique
*********************************************************/
LightingTechnique::LightingTechnique() {
	m_vertex_shader_path = "shaders/lighting_technique.vert";
	m_fragment_shader_path = "shaders/lighting_technique.frag";
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
	m_vertex_shader_path = "shaders/billboard.vert";
	m_geometry_shader_path = "shaders/billboard.geom";
	m_fragment_shader_path = "shaders/billboard.frag";
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

void BillboardTechnique::set_vp_trans(const M3DMatrix44f vp) {
	glUniformMatrix4fv(m_vp_location, 1, false, vp);
}

void BillboardTechnique::set_camera_position(const M3DVector3f cam_pos) {
	glUniform3f(m_cam_pos_location, cam_pos[0], cam_pos[1], cam_pos[2]);
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
	m_vertex_shader_path = "shaders/ps_update.vert";
	m_geometry_shader_path = "shaders/ps_update.geom";
	m_fragment_shader_path = "shaders/ps_update.frag";
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
	m_vertex_shader_path = "shaders/picking.vert";
	m_fragment_shader_path = "shaders/picking.frag";
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
void PickingTechnique::set_wvp(const M3DMatrix44f wvp) {
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp);
}

void PickingTechnique::on_draw_start_callback(unsigned int mesh_id) {
	set_mesh_id(mesh_id);
}

/*********************************************************
Simple Color Technique
*********************************************************/
SimpleColorTechnique::SimpleColorTechnique() {
	m_vertex_shader_path = "shaders/simple_color.vert";
	m_fragment_shader_path = "shaders/simple_color.frag";
}
SimpleColorTechnique::~SimpleColorTechnique() {

}

bool SimpleColorTechnique::init() {
	if (!Technique::init()) return false;

	m_wvp_location = glGetUniformLocation(m_program_id, "wvp");

	return m_wvp_location != INVALID_UNIFORM_LOCATION;
	
}
void SimpleColorTechnique::set_wvp(const M3DMatrix44f wvp) {
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp);
}

/*********************************************************
Simple Show Technique
*********************************************************/

SimpleShowTechnique::SimpleShowTechnique() {
	m_vertex_shader_path = "shaders/simple_show.vert";
	m_fragment_shader_path = "shaders/simple_show.frag";
}
SimpleShowTechnique::~SimpleShowTechnique() {

}

bool SimpleShowTechnique::init() {
	if (!Technique::init()) return false;

	m_wvp_location = glGetUniformLocation(m_program_id, "wvp");
	m_color_sampler_location = glGetUniformLocation(m_program_id, "g_color_sampler");

	return m_wvp_location != INVALID_UNIFORM_LOCATION && m_color_sampler_location != INVALID_UNIFORM_LOCATION;
}
void SimpleShowTechnique::set_wvp(const M3DMatrix44f wvp) {
	glUniformMatrix4fv(m_wvp_location, 1, GL_FALSE, wvp);
}
void SimpleShowTechnique::set_tex_index(unsigned int tex_index) {
	glUniform1i(m_color_sampler_location, tex_index);
}

/*********************************************************
Tessellation Technique
*********************************************************/
TessellationTechnique::TessellationTechnique() {
	m_vertex_shader_path = "shaders/tessellation.vert";
	m_tc_shader_path = "shaders/tessellation.tessc";
	m_te_shader_path = "shaders/tessellation.tesse";
	m_fragment_shader_path = "shaders/tessellation.frag";
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
void TessellationTechnique::set_world(const M3DMatrix44f w) {
	glUniformMatrix4fv(m_world_location, 1, false, w);
}
void TessellationTechnique::set_vp(const M3DMatrix44f vp) {
	glUniformMatrix4fv(m_vp_location, 1, false, vp);
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
void TessellationTechnique::set_eye_pos(const M3DVector3f eye_pos) {
	glUniform3f(m_eye_pos, eye_pos[0], eye_pos[1], eye_pos[2]);
}
void TessellationTechnique::set_specular_param(float intensity, float power) {
	glUniform1f(m_specular_intensity, intensity);
	glUniform1f(m_specular_power, power);
}