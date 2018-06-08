#ifndef _TUTORIAL_PARTICAL_SYSTEM_H
#define _TUTORIAL_PARTICAL_SYSTEM_H

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/technique.h"
#include <vector>
#include "common/skybox.h"
#include "common/billboard_list.h"
#include "common/partical_system.h"

class TutorialParticalSystem : public ICallback {
public:
	bool init();

	// ��Ⱦ����
	void render_scene_callback(float width, float height, float time);

	// ���̻ص�
	void key_callback(int key, int scancode, int action, int mods);

	// ����ƶ��ص�
	void cursor_position_callback(double x, double y);

protected:
	void render_pass(float time);

private:
	Camera * m_cam;
	PersProjInfo m_proj_info;

	Skybox* m_skybox;
	ParticalSystem* m_partical_system;

	float m_pre_time;
};

#endif // !_TUTORIAL_PARTICAL_SYSTEM_H
