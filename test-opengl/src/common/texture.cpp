#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"
#include <stdio.h>
#include <random>
#include "math3d/math3d.h"
#include <vector>

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
		printf("loading file %s failed\n", m_filename.c_str());
		return false;
	}
	printf("loading file %s success\n", m_filename.c_str());
	

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
/***********************************************
CubemapTexture
**********************************************/
static const GLenum cube_map_types[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

CubemapTexture::CubemapTexture(
	const std::string& pox_x_filepath,
	const std::string& pox_y_filepath,
	const std::string& pox_z_filepath,
	const std::string& neg_x_filepath,
	const std::string& neg_y_filepath,
	const std::string& neg_z_filepath
) {
	m_filepaths[0] = pox_x_filepath;
	m_filepaths[1] = pox_y_filepath;
	m_filepaths[2] = pox_z_filepath;
	m_filepaths[3] = neg_x_filepath;
	m_filepaths[4] = neg_y_filepath;
	m_filepaths[5] = neg_z_filepath;
	m_texture_obj = 0;
}
CubemapTexture::~CubemapTexture() {
	if (m_texture_obj != 0) {
		glDeleteTextures(1, &m_texture_obj);
	}
}
bool CubemapTexture::load() {
	glGenTextures(1, &m_texture_obj);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);
	for (int i = 0; i < 6; ++i) {
		int width, height, channels;
		unsigned char* data = stbi_load(m_filepaths[i].c_str(), &width, &height, &channels, 4);
		if (data == NULL) {
			printf("loading file %s failed\n", m_filepaths[i].c_str());
			continue;
		}
		printf("loading file %s success\n", m_filepaths[i].c_str());
		glTexImage2D(cube_map_types[i], 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		stbi_image_free(data);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}
void CubemapTexture::bind(GLenum texture_unit) {
	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture_obj);
}

RandomTexture::RandomTexture()
{
	m_texture_obj = GL_INVALID_VALUE;
}

RandomTexture::~RandomTexture()
{
	if (m_texture_obj != GL_INVALID_VALUE) {
		glDeleteTextures(1, &m_texture_obj);
	}
}

void RandomTexture::bind(GLenum texture_unit) {
	glActiveTexture(texture_unit);
	glBindTexture(GL_TEXTURE_1D, texture_unit);
}
bool RandomTexture::init(unsigned int size) {
#define RAND_VAL (float)rand() / RAND_MAX
	std::vector<M3DVector3f> vals(size);
	for (int i = 0; i < size; ++i) {
		M3DVector3f pos;
		pos[0] = RAND_VAL;
		pos[1] = RAND_VAL;
		pos[2] = RAND_VAL;
	}
#undef RAND_VAL
	
	glGenTextures(1, &m_texture_obj);
	glBindTexture(GL_TEXTURE_1D, m_texture_obj);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, &vals);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	return glGetError() == GL_NO_ERROR;
}