#ifndef _TUTORIAL_PRACTICE_H
#define _TUTORIAL_PRACTICE_H
#include "i_callbacks.h"
#include "technique.h"
#include "texture.h"
#include "math3d\math3d.h"
#include "pipline.h"
#include "camera.h"
#include "mesh.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 750


class SimpleTechnique : public Technique {
public:
	bool init() {
		if (!Technique::init()) return false;
		if (!add_shader(GL_VERTEX_SHADER, "shaders/tutorial_2.vs")) return false;
		if (!add_shader(GL_FRAGMENT_SHADER, "shaders/tutorial_2.ps")) return false;
		if (!finalize()) return false;
		return true;
	}
};


// ���������
class AmbianceLightTechnique : public Technique {
public:
	bool init();
	void set_ambiance_light();
	void set_wvp_trans(M3DMatrix44f wvp);

public:
	M3DVector3f m_ambiance_color;
	float m_ambiance_intensity;

protected:
	GLuint wvp_location;
	GLuint m_ambiance_color_location;
	GLuint m_ambiance_intensity_location;
};

class TutorialPractice : public ICallback {
public:
	TutorialPractice();
	~TutorialPractice();

	bool init();

	// ��Ⱦ����
	void render_scene_callback(float width, float height, float time);

	// ���̻ص�
	void key_callback(int key, int scancode, int action, int mods);

	// ����ƶ��ص�
	void cursor_position_callback(double x, double y);

private:
	Mesh* m_mesh;
	AmbianceLightTechnique* m_tech;
	Pipline* pipline;
	float width, height;
	PersProjInfo m_pp_info;
	OrthorProjInfo m_op_info;
	Camera* m_camera;

};

#endif // !_TUTORIAL_PRACTICE_H
