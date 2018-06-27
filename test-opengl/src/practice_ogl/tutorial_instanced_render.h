#ifndef _TUTORIAL_INSTANCED_RANDERING_H
#define _TUTORIAL_INSTANCED_RANDERING_H

#include "common/i_callbacks.h"
#include "common/camera.h"
#include "common/technique.h"
#include <vector>
#include "common/mesh.h"
#include "common/pipline.h"

class TutorialInstancedRendering : public ICallback {
public:
	TutorialInstancedRendering();
	~TutorialInstancedRendering();

	bool init();

	// ��Ⱦ����
	void render_scene_callback(float width, float height, float time);

	// ���̻ص�
	void key_callback(int key, int scancode, int action, int mods);

	// ����ƶ��ص�
	void cursor_position_callback(double x, double y);

	void init_positions();

private:
	Camera* m_cam;
	PersProjInfo m_proj_info;

	VAOMesh* m_mesh;
	InstanceRenderingTechnique* m_tech;

	Vector3* m_positions;
};

#endif // !_TUTORIAL_INSTANCED_RANDERING_H
