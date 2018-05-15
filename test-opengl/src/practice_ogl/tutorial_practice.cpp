#include <math.h>
#include "tutorial_practice.h"
#include "glfw\include\glfw3.h"


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

// 初始化
TutorialPractice::TutorialPractice() {
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
void TutorialPractice::render_scene_callback(float width, float height, float delta_time) {

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

// 键盘回调
void TutorialPractice::key_callback(int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS) return;

	switch (key)
	{
	case GLFW_KEY_UP:
		printf("up is pressed\n");
		break;
	default:
		break;
	}
}

// 销毁
TutorialPractice::~TutorialPractice() {
	glDeleteBuffers(1, &vbo);
	delete texture;
	delete tech;
}