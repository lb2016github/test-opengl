#include "partical_system.h"
#include "glad/glad.h"
#include "const.h"

ParticalSystem::ParticalSystem()
{
	m_billboard_tech = NULL;
	m_ps_update_tech = NULL;
	m_rand_texture = NULL;
	m_color_texture = NULL;
	m_time = 0;
	m_first = true;
	m_cur_vb = 0;
	m_cur_tfb = 1;

	memset(m_tfos, 0, sizeof(m_tfos));
	memset(m_vbos, 0, sizeof(m_vbos));
}

ParticalSystem::~ParticalSystem()
{
	if (m_billboard_tech != NULL) {
		delete m_billboard_tech;
	}
	if (m_ps_update_tech != NULL) {
		delete m_ps_update_tech;
	}
	if (m_rand_texture != NULL) {
		delete m_rand_texture;
	}
	if (m_color_texture != NULL) {
		delete m_color_texture;
	}

	if (m_tfos[0] != 0) {
		glDeleteTransformFeedbacks(2, m_tfos);
	}
	if (m_vbos[0] != 0) {
		glDeleteBuffers(2, m_vbos);
	}
}

bool ParticalSystem::init() {
	m_billboard_tech = new BillboardTechnique();
	bool rst = m_billboard_tech->init();
	if (!rst) {
		printf("ERROR: init BillboardList failed\n");
		return false;
	}
	m_billboard_tech->enable();
	m_billboard_tech->set_billboard_size(0.1, 0.1);
	m_billboard_tech->set_color_map_tex_unit(COLOR_TEXTURE_UNIT_INDEX);

	m_ps_update_tech = new PSUpdateTechnique();
	rst = m_ps_update_tech->init();
	if (!rst) {
		printf("ERROR: init PSUpdateTechnique failed\n");
		return false;
	}
	m_ps_update_tech->enable();
	m_ps_update_tech->set_life_time(100, 10000, 2500);
	m_ps_update_tech->set_random_texture_unit(RANDOM_TEXTURE_UNIT_INDEX);

	m_rand_texture = new RandomTexture();
	rst = m_rand_texture->init(1000);
	if (!rst) {
		printf("ERROR: init RandomTexture failed\n");
		return false;
	}

	m_color_texture = new Texture(GL_TEXTURE_2D, "res/fireworks_red.jpg");
	m_color_texture->load();


	Partical particals[MAX_PARTICLES];
	memset(particals, 0, sizeof(particals));
	particals[0].type = PARTICLE_TYPE_LAUNCHER;
	particals[0].age = 0;
	m3dLoadVector3(particals[0].position, 0, 0, 0);
	m3dLoadVector3(particals[0].velocity, 0, 0.001, 0);

	glGenTransformFeedbacks(2, m_tfos);
	glGenBuffers(2, m_vbos);
	for (int i = 0; i < 2; ++i) {
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_tfos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbos[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particals), particals, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbos[i]);
	}


}
void ParticalSystem::render(float delta_time, const M3DVector3f cam_pos, const M3DMatrix44f vp) {
	m_time += delta_time;
	ps_update(delta_time);
	ps_render(cam_pos, vp);
	GLuint tmp = m_cur_vb;
	m_cur_vb = m_cur_tfb;
	m_cur_tfb = tmp;
}


void ParticalSystem::ps_update(float delta_time) {
	m_rand_texture->bind(RANDOM_TEXTURE_UNIT);
	m_ps_update_tech->enable();
	m_ps_update_tech->set_delta_time(delta_time);
	m_ps_update_tech->set_time(m_time);

	//printf("(time, delta_time): (%f, %f)\n", m_time, delta_time);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[m_cur_vb]);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Partical), (GLvoid*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Partical), (GLvoid*)4);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Partical), (GLvoid*)8);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Partical), (GLvoid*)20);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_tfos[m_cur_tfb]);
	glBeginTransformFeedback(GL_POINTS);
	if (m_first) {
		glDrawArrays(GL_POINTS, 0, 1);
		m_first = false;
	}
	else {
		glDrawTransformFeedback(GL_POINTS, m_tfos[m_cur_vb]);
	}
	glEndTransformFeedback();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	glDisable(GL_RASTERIZER_DISCARD);
}
void ParticalSystem::ps_render(const M3DVector3f cam_pos, const M3DMatrix44f vp) {
	m_color_texture->bind(COLOR_TEXTURE_UNIT);
	m_billboard_tech->enable();
	m_billboard_tech->set_camera_position(cam_pos);
	m_billboard_tech->set_vp_trans(vp);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbos[m_cur_tfb]);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(Partical), (GLvoid*)8);
	glDrawTransformFeedback(GL_POINTS, m_tfos[m_cur_tfb]);
	glDisableVertexAttribArray(0);
}