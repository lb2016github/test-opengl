#include "util\practice_header.h"
#include "util\util.h"
#include <glad\glad.h>
#include <math.h>
#include "math3d\math3d.h"
#include "texture.h"
#include "technique.h"

#define Vector3f M3DVector3f
#define Vector2f M3DVector2f

GLuint vbo, ibo, program_id;
Texture *texture;
int ele_len = 0;


class SimpleTechnique : public Technique {
public:
	bool init() {
		if (!Technique::init()) return false;
		if (!add_shader(GL_VERTEX_SHADER, "shaders/tutorial_2.vs")) return false;
		if (!add_shader(GL_FRAGMENT_SHADER, "shaders/tutorial_2.ps")) return false;
		if (!finalize()) return false;
		return true;
	}
	void set_uniform(char* u_name, float value) {
		GLuint loc = glGetUniformLocation(m_program_id, u_name);
		glUniform1f(loc, value);
	}
	void set_uniform(char* u_name, bool trans, M3DMatrix33f mtx) {
		GLuint loc = glGetUniformLocation(m_program_id, u_name);
		glUniformMatrix3fv(loc, 1, false, mtx);
	}
};

SimpleTechnique* tech;

static const GLfloat vertex_data[] = {
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
};

static const GLuint index_data[] = {
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
};

struct Vectex {
public:
	Vector3f m_pos;
	Vector2f m_tex;

	Vectex(Vector3f pos, Vector2f tex) {
		for (int i = 0; i < 3; ++i) m_pos[i] = pos[i];
		for (int i = 0; i < 2; ++i) m_tex[i] = tex[i];
	}
};

// 初始化
void gl_init() {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);

	tech = new SimpleTechnique();
	tech->init();
	tech->enable();

	texture = new Texture(GL_TEXTURE_2D, "res/test.png");
	texture->load();
}

// 更新
void gl_update(float width, float height, float delta_time) {

	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertex_data[0]) * 5,
		(void*)0
	);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertex_data[0]) * 5,
		(const GLvoid*)(sizeof(vertex_data[0]) * 3)
	);

	float scale = sinf(delta_time);
	M3DMatrix33f rot_matrix;
	m3dRotationMatrix33(rot_matrix, delta_time, 1, 1, 1);
	tech->set_uniform("scale", scale);
	tech->set_uniform("rot_mat", false, rot_matrix);

	texture->bind(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

// 销毁
void gl_destroy() {
	glDeleteBuffers(1, &vbo);
	delete texture;
	delete tech;
}