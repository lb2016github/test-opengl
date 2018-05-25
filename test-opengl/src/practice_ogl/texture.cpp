#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include <stdio.h>

Texture::Texture(GLenum texture_target, const std::string& filename): data(NULL) {
	m_texture_target = texture_target;
	m_filename = filename;
}

bool Texture::load() {
	if (data != NULL) {
		stbi_image_free(data);
		data = NULL;
	}
	int width, height, channels;
	data = stbi_load(m_filename.c_str(), &width, &height, &channels, 4);
	if (data == NULL) {
		printf("loading file %s failed\n", m_filename);
		return false;
	}
	printf("loading file %s success\n", m_filename);
	

	glGenTextures(1, &m_texture_obj);
	glBindTexture(m_texture_target, m_texture_obj);
	glTexImage2D(m_texture_target, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glTexParameterf(m_texture_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(m_texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(m_texture_target, 0);

	return true;
}

void Texture::bind(GLenum texture_unit) {
	glActiveTexture(texture_unit);
	glBindTexture(m_texture_target, m_texture_obj);
}

Texture::~Texture() {
	if (data != NULL) {
		stbi_image_free(data);
		data = NULL;
	}
}