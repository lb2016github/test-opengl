#ifndef _PARTICAL_SYSTEM_H
#define _PARTICAL_SYSTEM_H
#include "technique.h"
#include "glad/glad.h"
#include "billboard_list.h"
#include "texture.h"
#include "math3d/math3d_ex.h"

#define MAX_PARTICLES 1000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0
#define PARTICLE_TYPE_SHELL 1.0
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0

struct Partical {
	float type;
	float age;
	Vector3 position;
	Vector3 velocity;
};

class ParticalSystem
{
public:
	ParticalSystem();
	~ParticalSystem();

	bool init();
	void render(float delta_time, const Vector3& cam_pos, const Matrix& vp);

private:
	void ps_update(float delta_time);
	void ps_render(const Vector3& cam_pos, const Matrix& vp);

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
