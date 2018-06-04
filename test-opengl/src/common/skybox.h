#ifndef _SKYBOX_H
#define _SKYBOX_H
#include "texture.h"
#include "technique.h"
#include "glad/glad.h"
#include "math3d/math3d.h"
#include "camera.h"
#include "pipline.h"
#include "mesh.h"

class SkyboxTechnique : public Technique {
public:
	SkyboxTechnique();
	~SkyboxTechnique();
	virtual bool init();

	void set_wvp_trans(const M3DMatrix44f wvp);
	void set_texture_unit(unsigned int texture_unit);

protected:
	virtual void init_shader_path();

private:
	GLuint m_wvp_location;
	GLuint m_sampler_location;
};

class Skybox {
public:
	Skybox(const Camera* cam, const PersProjInfo& p);
	~Skybox();
	bool init(
		const std::string& pox_x_filepath,
		const std::string& pox_y_filepath,
		const std::string& pox_z_filepath,
		const std::string& neg_x_filepath,
		const std::string& neg_y_filepath,
		const std::string& neg_z_filepath
	);
	void render();
private:
	CubemapTexture * m_skybox_tex;
	SkyboxTechnique* m_skybox_technique;

	const Camera* m_cam;
	PersProjInfo m_pers_proj_info;

	Mesh* m_mesh;
};

#endif // !_SKYBOX_H
