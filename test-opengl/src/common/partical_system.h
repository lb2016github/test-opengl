#ifndef _PARTICAL_SYSTEM_H
#define _PARTICAL_SYSTEM_H
#include "technique.h"
#include "glad/glad.h"
#include "billboard_list.h"
#include "texture.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0
#define PARTICLE_TYPE_SHELL 1.0
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0

struct Partical {
	float type;
	float age;
	M3DVector3f position;
	M3DVector3f velocity;
};

class ParticalSystem
{
public:
	ParticalSystem();
	~ParticalSystem();

	bool init();
	void render(float time, const M3DVector3f cam_pos, const M3DMatrix44f vp);

private:
	void ps_update(float delta_time);
	void ps_render(const M3DVector3f cam_pos, const M3DMatrix44f vp);

private:
	BillboardTechnique * m_billboard_tech;
	PSUpdateTechnique * m_ps_update_tech;

	RandomTexture* m_rand_texture;
	Texture* m_color_texture;

	GLuint m_tfos[2];
	GLuint m_vbos[2];
	unsigned int m_cur_vb;
	unsigned int m_cur_tfb;

	float m_time;
	bool m_first;
};

#endif // !_PARTICAL_SYSTEM_H
