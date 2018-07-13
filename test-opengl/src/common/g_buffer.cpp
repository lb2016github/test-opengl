#include "g_buffer.h"
#include "utils.h"
#include <stdio.h>

GBuffer::GBuffer() {
	m_fbo = 0;
	m_depth_texture = 0;
	memset(m_textures, 0, sizeof(m_textures));
}
GBuffer::~GBuffer() {
	if (m_fbo != 0) glDeleteFramebuffers(1, &m_fbo);
	if (m_depth_texture != 0) glDeleteTextures(1, &m_depth_texture);
	if (m_textures[0] != 0) glDeleteTextures(GBUFFER_NUM_TEXTURES, m_textures);
}

bool GBuffer::init(unsigned int win_width, unsigned int win_height) {
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	glGenTextures(GBUFFER_NUM_TEXTURES, m_textures);
	for (int i = 0; i < GBUFFER_NUM_TEXTURES; ++i) {
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, win_width, win_height, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_textures[i], 0);
	}

	glGenTextures(1, &m_depth_texture);
	glBindTexture(GL_TEXTURE_2D, m_depth_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, win_width, win_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth_texture, 0);

	GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, draw_buffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status: 0x%x\n", Status);
		return false;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;
}
void GBuffer::bind_for_writing() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}
void GBuffer::bind_for_reading() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	for (int i = 0; i < GBUFFER_NUM_TEXTURES; ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
	}
}
void GBuffer::set_read_buffer(GBUFFER_TEXTURE_TYPE texture_type) {
	glReadBuffer(GL_COLOR_ATTACHMENT0 + texture_type);
}