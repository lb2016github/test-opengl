#ifndef _TUTORIAL_PRACTICE_H
#define _TUTORIAL_PRACTICE_H
#include "common/const.h"
#include "common/i_callbacks.h"
#include "common/technique.h"
#include "common/texture.h"
#include "math3d\math3d.h"
#include "common/pipline.h"
#include "common/camera.h"
#include "common/mesh.h"


class TutorialPractice : public ICallback {
public:
	TutorialPractice();
	~TutorialPractice();

	bool init();

	// 渲染场景
	void render_scene_callback(float width, float height, float time);

	// 键盘回调
	void key_callback(int key, int scancode, int action, int mods);

	// 光标移动回调
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
