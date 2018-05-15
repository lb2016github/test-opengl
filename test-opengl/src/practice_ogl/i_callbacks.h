#ifndef _I_CALLBACK_H
#define _I_CALLBACK_H

class ICallback {
public:
	// 键盘回调
	virtual void key_callback(int key, int scancode, int action, int mods) {};

	// 鼠标点击回调
	virtual void mouse_button_callback(int button, int action, int mods) {};

	// 光标移动回调
	virtual void cursor_position_callback(double x, double y) {};

	// 滚轮回调
	virtual void scroll_callback(double x, double y) {};

	// 渲染场景
	virtual void render_scene_callback(float width, float height, float time) {};

	// 错误的回调
	virtual void error_callback(int error, const char* description) {};
};


#endif // _I_CALLBACK_H
