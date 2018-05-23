#ifndef _TECHNIQUE_H
#define _TECHNIQUE_H
#include "glad\glad.h"
#include <list>
#include "math3d/math3d.h"


class Technique {
public:
	Technique();
	virtual ~Technique();
	virtual bool init();
	void enable();

protected:
	bool add_shader(GLenum shader_type, const char* filename);	// add shader object
	bool finalize();	// compile and link shader
	GLuint m_program_id;

private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shader_obj_list;

};

/*********************************************************
Direction light Technique
*********************************************************/

struct DirectionLight {
public:
	M3DVector3f m_color;
	float m_ambiance_intensity;
	float m_diffuse_intensity;
	M3DVector3f m_direction;
};

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
	GLuint wvp_location, w_locatioin;
	GLuint m_color_location, m_ambiance_intensity_location, m_diffuse_intensity_location, m_direction_location;
	GLuint m_sampler_location;
	GLuint m_eye_pos_location;
	GLuint m_materia_specular_intensity_location;
	GLuint m_specular_power;
};


/*********************************************************
Spot light Technique
*********************************************************/

#endif // !_TECHNIQUE_H
