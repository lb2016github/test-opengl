#include "shadow_map_fbo.h"
#include "const.h"
#include <stdio.h>

ShadowMapFBO::ShadowMapFBO() {
	m_fbo = 0;
	m_shadow_map = 0;
}
ShadowMapFBO::~ShadowMapFBO() {
	if (m_fbo != 0) {
		glDeleteFramebuffers(1, &m_fbo);
	}
	if (m_shadow_map != 0) {
		glDeleteTextures(1, &m_shadow_map);
	}
}

bool ShadowMapFBO::init(unsigned int window_width, unsigned int window_height) {
	glGenFramebuffers(1, &m_fbo);

	// create texture obj
	glGenTextures(1, &m_shadow_map);
	glBindTexture(GL_TEXTURE_2D, m_shadow_map);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, window_width, window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// create frame buffer

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadow_map, 0);

	// disable writes to the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glEnable(GL_DEPTH_TEST);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		printf("FB error, status 0x%x\n", status);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;

}
void ShadowMapFBO::bind_for_writing() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}
void ShadowMapFBO::bind_for_reading(GLenum texture_unit) {
	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_2D, m_shadow_map);
}