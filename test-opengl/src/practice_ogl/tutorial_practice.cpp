#include "util\practice_header.h"
#include "util\util.h"
#include <glad\glad.h>


GLuint vbo, program_id;

static const GLfloat vertex_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

// 初始化
void gl_init() {
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

	program_id = LoadShaders("tutorial_2.vs", "tutorial_2.ps");
	glUseProgram(program_id);
}

// 更新
void gl_update(float width, float height, float delta_time) {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(0);

}

// 销毁
void gl_destroy() {
	glDeleteBuffers(1, &vbo);
}