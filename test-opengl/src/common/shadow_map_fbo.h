#ifndef _SHADOW_MAP_FBO_H
#define _SHADOW_MAP_FBO_H
#include "glad/glad.h"


class ShadowMapFBO {
public:
	ShadowMapFBO();
	~ShadowMapFBO();

	bool init(unsigned int window_width, unsigned int window_height);
	void bind_for_writing();
	void bind_for_reading(GLenum texture_unit);

private:
	GLuint m_fbo;
	GLuint m_shadow_map;
};

#endif // !_SHADOW_MAP_FBO_H
