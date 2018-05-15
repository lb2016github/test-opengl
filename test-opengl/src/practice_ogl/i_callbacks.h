#ifndef _I_CALLBACK_H
#define _I_CALLBACK_H

class ICallback {
public:
	// ���̻ص�
	virtual void key_callback(int key, int scancode, int action, int mods) {};

	// ������ص�
	virtual void mouse_button_callback(int button, int action, int mods) {};

	// ����ƶ��ص�
	virtual void cursor_position_callback(double x, double y) {};

	// ���ֻص�
	virtual void scroll_callback(double x, double y) {};

	// ��Ⱦ����
	virtual void render_scene_callback(float width, float height, float time) {};

	// ����Ļص�
	virtual void error_callback(int error, const char* description) {};
};


#endif // _I_CALLBACK_H
