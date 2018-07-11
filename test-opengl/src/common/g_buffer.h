#ifndef _G_BUFFER_H
#define _G_BUFFER_H
#include "glad/glad.h"

#define G_BUFFER_POSITION_TEXTURE_UNIT 0
#define G_BUFFER_NORMAL_TEXTURE_UNIT 1
#define G_BUFFER_DIFFUSE_TEXTURE_UNIT 2
#define G_BUFFER_TEX_COORD_TEXTURE_UNIT 3

class GBuffer {
public:
	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,
		GBUFFER_TEXTURE_TYPE_NORMAL,
		GBUFFER_TEXTURE_TYPE_DIFFUSE,
		GBUFFER_TEXTURE_TYPE_TEXCOORD,
		GBUFFER_NUM_TEXTURES
	};

	GBuffer();
	~GBuffer();

	bool init(unsigned int win_width, unsigned int win_height);
	void bind_for_writing();
	void bind_for_reading();
	void set_read_buffer(GBUFFER_TEXTURE_TYPE texture_type);

private:
	GLuint m_fbo;
	GLuint m_textures[GBUFFER_NUM_TEXTURES];
	GLuint m_depth_texture;
};

#endif // !_G_BUFFER_H
