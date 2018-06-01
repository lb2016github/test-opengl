#ifndef _TECHNIQUE_H
#define _TECHNIQUE_H
#include "glad\glad.h"
#include <list>
#include "math3d/math3d.h"
#include "light.h"
#include <vector>

#define MAX_POINT_LIGHT_COUNT 2
#define MAX_SPOT_LIGHT_COUNT 2

class Technique {
public:
	Technique();
	virtual ~Technique();
	virtual bool init();
	void enable();

protected:
	virtual void init_shader_path()=0;

	bool add_shader(GLenum shader_type, const char* filename);	// add shader object
	bool finalize();	// compile and link shader
	GLuint m_program_id;
	std::string m_vertex_shader_path;
	std::string m_fragment_shader_path;

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
};

struct DirectionLightLocation : BaseLightLocation {
	GLuint direction;

	bool init_location(
		const GLuint program_id,
		const char* color_name, const char* ambiant_intensity_name, const char* diffuse_intensity_name, const char* direction_name
	);

	virtual void set_light(DirectionLight& light);
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
	bool init();
	void set_light(DirectionLight& m_dir_light);
	void set_transformation(M3DMatrix44f wvp, M3DMatrix44f w);
	void set_texture_unit(int unit_idx);
	void set_eye_pos(M3DVector3f eye_pos);
	void set_material_specular_intensity(float materia);
	void set_specular_power(float power);

protected:
	virtual void init_shader_path();

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
	bool init();

	void set_direction_light(DirectionLight& dir_light);
	void set_point_lights(std::vector<PointLight>& point_light_list);
	void set_eye_position(M3DVector3f eye_pos);
	void set_specular_parameter(float spec_intensity, float spec_pow);
	void set_transformation(M3DMatrix44f wvp, M3DMatrix44f world_trans);
	void set_texture_unit(int unit_idx);

protected:
	virtual void init_shader_path();

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
	bool init();
	void set_spot_lights(std::vector<SpotLight>& spot_light_list);

protected:
	virtual void init_shader_path();

private:
	SpotLightLocation m_spot_light_locations[MAX_SPOT_LIGHT_COUNT];
	GLuint m_spot_light_num_location;
};

#endif // !_TECHNIQUE_H
