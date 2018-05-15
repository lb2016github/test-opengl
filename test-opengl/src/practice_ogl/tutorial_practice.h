#ifndef _TUTORIAL_PRACTICE_H
#define _TUTORIAL_PRACTICE_H
#include "i_callbacks.h"
#include "technique.h"
#include "texture.h"
#include "math3d\math3d.h"

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

class TutorialPractice : public ICallback {
public:
	TutorialPractice();
	~TutorialPractice();

	// äÖÈ¾³¡¾°
	void render_scene_callback(float width, float height, float time);

	// ¼üÅÌ»Øµ÷
	void key_callback(int key, int scancode, int action, int mods);

private:
	GLuint vbo, ibo;
	Texture *texture;
	SimpleTechnique* tech;
};

#endif // !_TUTORIAL_PRACTICE_H
