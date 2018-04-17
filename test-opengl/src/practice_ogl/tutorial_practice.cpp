#include "util\practice_header.h"
#include "util\util.h"
#include <glad\glad.h>
#include <math.h>
#include "math3d\math3d.h"


GLuint vbo, ibo, program_id;

static const GLfloat vertex_data[] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f,
};

static const GLuint index_data[] = {
	0, 3, 1,
	1, 3, 2,
	2, 3, 0,
	0, 1, 2
};

// 初始化
void gl_init() {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data), index_data, GL_STATIC_DRAW);

	program_id = LoadShaders("shaders/tutorial_2.vs", "shaders/tutorial_2.ps");
	glUseProgram(program_id);
}

// 更新
void gl_update(float width, float height, float delta_time) {

	glClear(GL_COLOR_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	GLint scale_loc = glGetUniformLocation(program_id, "scale");
	GLint rot_mtx = glGetUniformLocation(program_id, "rot_mat");
	float scale = sinf(delta_time);
	M3DMatrix33f rot_matrix;
	m3dRotationMatrix33(rot_matrix, delta_time, 1, 1, 1);
	glUniform1f(scale_loc, scale);
	glUniformMatrix3fv(rot_mtx, 1, false, rot_matrix);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
}

// 销毁
void gl_destroy() {
	glDeleteBuffers(1, &vbo);
}