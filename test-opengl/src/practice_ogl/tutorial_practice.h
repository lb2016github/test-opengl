#ifndef _TUTORIAL_PRACTICE_H
#define _TUTORIAL_PRACTICE_H
#include "const.h"
#include "i_callbacks.h"
#include "technique.h"
#include "texture.h"
#include "math3d\math3d.h"
#include "pipline.h"
#include "camera.h"
#include "mesh.h"


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
	IMesh* m_mesh;
	DirectionLightTechnique* m_tech;
	Pipline* pipline;
	float width, height;
	PersProjInfo m_pp_info;
	OrthorProjInfo m_op_info;
	Camera* m_camera;
	DirectionLight m_dir_light;
};

#endif // !_TUTORIAL_PRACTICE_H
