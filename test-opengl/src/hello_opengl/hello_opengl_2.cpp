#include <hello_opengl\hello_opengl.h>
#include <glad\glad.h>
#include "linmath.h"

/************************************************
init data
************************************************/
static const struct vertex{
	float x, y, z;
	float r, g, b;
}vertices[4] = {
	{-0.5f, 0.f, 0.f, 0.f, 0.f, 1.f},
	{0.5f, 0.f, 0.f, 0.f, 1.f, 0.f},
	{0.f, 0.86f, 0.f, 1.f, 0.f, 0.f},
	{0.f, 0.27f, 0.86f, 1.f, 1.f, 0.f},
};

short indices[4][3] = {
	{0.f, 1.f, 2.f},
	{0.f, 2.f, 3.f},
	{0.f, 1.f, 3.f},
	{1.f, 2.f, 3.f},
};

static const char* vertex_shader_text = 
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec3 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 1.0);\n"
"    color = vCol;\n"
"}\n";


static const char* fragment_shader_text =
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

/************************************************
end init data
************************************************/

GLuint vbo, ebo, vertex_shader, fragment_shader, progrem;
GLint mvp_location, vpos_location, vc_location;

#define BUFFER_OFFSET(offset) ((void*)(offset))

// 初始化
void gl_init() {
	//生成缓存数据
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);

	progrem = glCreateProgram();
	glAttachShader(progrem, vertex_shader);
	glAttachShader(progrem, fragment_shader);
	glLinkProgram(progrem);

	mvp_location = glGetUniformLocation(progrem, "MVP");
	vpos_location = glGetAttribLocation(progrem, "vPos");
	vc_location = glGetAttribLocation(progrem, "vCol");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(vc_location);
	glVertexAttribPointer(vc_location, 3, GL_FLOAT, GL_FALSE,
		sizeof(float) * 6, BUFFER_OFFSET(sizeof(float) * 3));
}

// 更新
void gl_update(float width, float height, float cur_time) {
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	mat4x4 m, p, mvp;


	float ratio = width / height;

	mat4x4_identity(m);
	mat4x4_rotate_Z(m, m, cur_time);
	mat4x4_rotate_Y(m, m, cur_time);
	mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	mat4x4_mul(mvp, p, m);

	glUseProgram(progrem);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*)mvp);
	//glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawElements(GL_LINE_STRIP, 4 * 3, GL_UNSIGNED_SHORT, NULL);
}

// 销毁
void gl_destroy() {
	return;
}