#ifndef _TECHNIQUE_H
#define _TECHNIQUE_H
#include "glad\glad.h"
#include <list>
#include "math3d/math3d_ex.h"
#include "light.h"
#include <vector>
#include "i_callbacks.h"

#define MAX_POINT_LIGHT_COUNT 2
#define MAX_SPOT_LIGHT_COUNT 2

class Technique {
public:
	Technique();
	Technique(
		const std::string& vertex_shader_path,
		const std::string& tsc_shader_path,
		const std::string& tse_shader_path,
		const std::string& geometry_shader_path,
		const std::string& fragment_shader_path
	);
	virtual ~Technique();
	virtual bool init();
	void enable();

protected:
	bool add_shader(GLenum shader_type, const char* filename);	// add shader object
	bool add_shader(GLenum shader_type, const std::string& filename);	// add shader object
	bool finalize();	// compile and link shader
	GLuint m_program_id;
	std::string m_vertex_shader_path;
	std::string m_tc_shader_path;
	std::string m_te_shader_path;
	std::string m_fragment_shader_path;
	std::string m_geometry_shader_path;

private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shader_obj_list;

};


/*********************************************************
Locations
*********************************************************/
struct BaseLightLocation {
	GLuint color;
	GLuint ambiant_intensity;
	GLuint diffuse_intensity;

	bool init_location(
		const GLuint program_id,
		const char* color_name, const char* ambiant_intensity_name, const char* diffuse_intensity_name
	);

	virtual void set_light(BaseLight& light);

	virtual bool is_valide();
};

struct DirectionLightLocation : BaseLightLocation {
	GLuint direction;

	bool init_location(
		const GLuint program_id,
		const char* color_name, const char* ambiant_intensity_name, const char* diffuse_intensity_name, const char* direction_name
	);

	virtual void set_light(DirectionLight& light);
	virtual bool is_valide();
};

struct PointLightLocation : BaseLightLocation {
	GLuint position;
	GLuint constant;
	GLuint linear;
	GLuint exp;

	bool init_location(
		const GLuint program_id,
		const char* color_name, const char* ambiant_intensity_name, const char* diffuse_intensity_name,
		const char* position_name, const char* constant_name, const char* linear_name, const char* exp_name
	);

	virtual void set_light(PointLight& light);
	virtual bool is_valide();
};

struct SpotLightLocation : PointLightLocation {
	GLuint direction;
	GLuint cutoff;

	bool init_location(
		const GLuint program_id,
		const char* color_name, const char* ambiant_intensity_name, const char* diffuse_intensity_name,
		const char* position_name, const char* constant_name, const char* linear_name, const char* exp_name,
		const char* direction_name, const char* cutoff_name
	);

	virtual void set_light(SpotLight& light);
};

/*********************************************************
Direction light Technique
*********************************************************/
// 环境光材质
class DirectionLightTechnique : public Technique {
public:
	DirectionLightTechnique();

	virtual bool init();
	void set_light(DirectionLight& m_dir_light);
	void set_transformation(Matrix& wvp, Matrix& w);
	void set_texture_unit(int unit_idx);
	void set_eye_pos(Vector3& eye_pos);
	void set_material_specular_intensity(float materia);
	void set_specular_power(float power);

protected:
	GLuint wvp_location, w_locatioin;
	GLuint m_color_location, m_ambiance_intensity_location, m_diffuse_intensity_location, m_direction_location;
	GLuint m_sampler_location;
	GLuint m_eye_pos_location;
	GLuint m_materia_specular_intensity_location;
	GLuint m_specular_power;
};


/*********************************************************
Point light Technique
*********************************************************/
class PointLightTechnique : public Technique {
public:
	PointLightTechnique();
	virtual bool init();

	void set_direction_light(DirectionLight& dir_light);
	void set_point_lights(std::vector<PointLight>& point_light_list);
	void set_point_light(PointLight& point_light);
	void set_eye_position(Vector3& eye_pos);
	void set_specular_parameter(float spec_intensity, float spec_pow);
	void set_transformation(Matrix& wvp, Matrix& world_trans);
	void set_texture_unit(int unit_idx);

private:
	DirectionLightLocation m_direction_light_location;
	PointLightLocation m_point_light_locations[MAX_POINT_LIGHT_COUNT];
	GLuint m_point_light_num_location;
	GLuint m_eye_world_pos_location;
	GLuint m_sampler_location;
	GLuint m_mat_specular_intensity_location;
	GLuint m_specular_power_location;
	GLuint m_wvp_location;
	GLuint m_w_location;

};


/*********************************************************
Spot light Technique
*********************************************************/
class SpotLightTechnique : public PointLightTechnique {
public:
	SpotLightTechnique();
	virtual bool init();
	void set_spot_lights(std::vector<SpotLight>& spot_light_list);

private:
	SpotLightLocation m_spot_light_locations[MAX_SPOT_LIGHT_COUNT];
	GLuint m_spot_light_num_location;
};

/*********************************************************
Shadowmap Technique
*********************************************************/
class ShadowMapTechnique : public SpotLightTechnique {
public:
	ShadowMapTechnique();

	virtual bool init();
	void set_light_wvp_trans(Matrix& light_wvp);
	void set_shadow_map_tex_unit(unsigned int tex_idx);

private:
	GLuint m_light_wvp_location;
	GLuint m_sampler_shadow_map_location;
};

/*********************************************************
Lighting Technique
*********************************************************/
class LightingTechnique : public ShadowMapTechnique {
public:
	LightingTechnique();
	virtual bool init();
	void set_normal_map_tex_unit(unsigned int texture_unit);

private:
	GLuint m_sampler_normal_map_location;
};

/*********************************************************
Billboard Technique
*********************************************************/
class BillboardTechnique : public Technique {
public:
	BillboardTechnique();
	virtual bool init();

	void set_vp_trans(const Matrix& vp);
	void set_camera_position(const Vector3& cam_pos);
	void set_billboard_size(float width, float height);
	void set_color_map_tex_unit(unsigned int tex_unit);

private:
	GLuint m_cam_pos_location;
	GLuint m_width_location;
	GLuint m_height_location;
	GLuint m_color_map_location;
	GLuint m_vp_location;
};


/*********************************************************
Partical System Update Technique
*********************************************************/
class PSUpdateTechnique: public Technique
{
public:
	PSUpdateTechnique();
	~PSUpdateTechnique();

	virtual bool init();
	void set_time(float time);
	void set_delta_time(float delta_time);
	void set_life_time(float lancher_life_time, float shell_life_time, float secondary_shell_life_time);
	void set_random_texture_unit(unsigned int texture_unit);

private:
	GLuint m_time_location;
	GLuint m_delta_time_location;
	GLuint m_lancher_life_time_location;
	GLuint m_shell_life_time_location;
	GLuint m_secondary_shell_life_time_location;
	GLuint m_random_texture_location;
};


/*********************************************************
Picking Technique
*********************************************************/
class PickingTechnique: public Technique, public IRenderCallback
{
public:
	PickingTechnique();
	~PickingTechnique();

	virtual bool init();
	void set_obj_id(unsigned int obj_id);
	void set_mesh_id(unsigned int mesh_id);
	void set_wvp(const Matrix& wvp);

	virtual void on_draw_start_callback(unsigned int mesh_id);

private:
	GLuint m_obj_id_location;
	GLuint m_mesh_id_location;
	GLuint m_wvp_location;
};


/*********************************************************
Simple Color Technique
*********************************************************/
class SimpleColorTechnique : public Technique
{
public:
	SimpleColorTechnique();
	~SimpleColorTechnique();

	virtual bool init();
	void set_wvp(const Matrix& wvp);

private:
	GLuint m_wvp_location;
};

/*********************************************************
Simple Show Technique
*********************************************************/
class SimpleShowTechnique : public Technique
{
public:
	SimpleShowTechnique();
	~SimpleShowTechnique();

	virtual bool init();
	void set_wvp(const Matrix& wvp);
	void set_tex_index(unsigned int tex_index);

private:
	GLuint m_wvp_location;
	GLuint m_color_sampler_location;
};

/*********************************************************
Tessellation Technique
*********************************************************/
class TessellationTechnique : public Technique
{
public:
	TessellationTechnique();
	~TessellationTechnique();

	virtual bool init();
	void set_world(const Matrix& wvp);
	void set_vp(const Matrix& vp);
	void set_tex_color_index(unsigned int color_index);
	void set_tess_level(float tess_level);
	void set_dir_light(DirectionLight& dir_light);
	void set_eye_pos(const Vector3& eye_pos);
	void set_specular_param(float intensity, float power);

private:
	GLuint m_world_location;
	GLuint m_vp_location;
	GLuint m_sampler_color_location;
	GLuint m_tess_level_location;
	DirectionLightLocation m_dir_light_location;
	GLuint m_eye_pos;
	GLuint m_specular_intensity;
	GLuint m_specular_power;

};

/*********************************************************
Tessellation Technique
*********************************************************/
class InstanceRenderingTechnique : public Technique
{
public:
	InstanceRenderingTechnique();
	~InstanceRenderingTechnique();

	virtual bool init();
	void set_tex_color_index(unsigned int color_index);

private:
	GLuint m_sampler_color_location;
};


/*********************************************************
Deferred shading
*********************************************************/
class DSGeometryTechnique : public Technique
{
public:
	DSGeometryTechnique();
	~DSGeometryTechnique();
	virtual bool init();
	void set_tex_color_index(unsigned int color_index);
	void set_wvp_trans(const Matrix& wvp);
	void set_world_trans(const Matrix& world);

private:
	GLuint m_color_sampler_location;
	GLuint m_wvp_location;
	GLuint m_world_location;
};

class DSLightTechnique : public Technique {
public:
	virtual bool init();
	void set_wvp_trans(const Matrix& wvp);
	void set_diffuse_sampler_index(unsigned int tex_index);
	void set_position_sampler_index(unsigned int tex_index);
	void set_normal_sampler_index(unsigned int tex_index);
	void set_window_size(int win_width, int win_height);
	void set_eye_pos(const Vector3 w_pos);
	void set_specular_param(float pow, float mat_specular);

private:
	GLuint m_wvp_location;
	GLuint m_diffuse_sampler_location;
	GLuint m_position_sampler_location;
	GLuint m_normal_sampler_location;
	GLuint m_win_size_location;
	GLuint m_eye_pos_location;
	GLuint m_specular_pow_location;
	GLuint m_specular_intensity_location;
};

class DSDirLightTechnique : public DSLightTechnique {
public:
	DSDirLightTechnique();
	~DSDirLightTechnique();
	virtual bool init();
	void set_dir_light(DirectionLight& dir_light);

private:
	DirectionLightLocation m_dir_light_location;
};

class DSPointLightTechnique : public DSLightTechnique {
public:
	DSPointLightTechnique();
	~DSPointLightTechnique();
	virtual bool init();
	void set_point_lights(int light_num, PointLight lights[], int start_index=0);
private:
	GLuint m_light_num_location;
	PointLightLocation m_light_locations[MAX_POINT_LIGHT_COUNT];
};

/*********************************************************
Stencil shadow volume
*********************************************************/
class SilhouetteDetectionTechnique : public Technique {
public:
	SilhouetteDetectionTechnique();
	~SilhouetteDetectionTechnique();

	virtual bool init();
	void set_wvp_mat(Matrix& wvp);
	void set_world_mat(Matrix& world_mat);
	void set_light_pos(Vector3& world_pos);

private:
	GLuint m_wvp_location;
	GLuint m_world_location;
	GLuint m_light_pos_location;
};

class NullTechnique : public Technique {
public:
	NullTechnique();
	~NullTechnique();
	bool init();
	void set_wvp_mtx(const Matrix& wvp);

private:
	GLuint m_wvp_location;
};

class StencilShadowVolumeTechnique : public Technique {
public:
	StencilShadowVolumeTechnique();
	~StencilShadowVolumeTechnique();

	bool init();
	void set_world_mtx(const Matrix& world_trans);
	void set_light_position(const Vector3& light_pos);
	void set_vp_mtx(const Matrix& vp_mtx);

private:
	GLuint m_world_trans_location;
	GLuint m_light_position_location;
	GLuint m_vp_location;
};

#endif // !_TECHNIQUE_H
