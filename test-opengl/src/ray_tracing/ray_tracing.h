#ifndef _RAY_TRAING_H
#define _RAY_TRACING_H
#include "common/i_callbacks.h"
#include "common/const.h"

/*
* ������
*/
class Scene {

};

/*
raytracing
*/
class RayTracingDemo: public ICallback
{
public:
	RayTracingDemo();
	~RayTracingDemo();

	// ��ʼ��
	void init();

	// ��Ⱦ����
	virtual void render_scene_callback(float width, float height, float time);
private:
	Scene scene;
};
#endif