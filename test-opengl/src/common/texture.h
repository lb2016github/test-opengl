#ifndef _TEXTURE_H
#define _TEXTURE_H
#include "glad\glad.h"
#include <string>


class Texture {
public:
	Texture(GLenum texture_target, const std::string& filename);

	bool load();

	void bind(GLenum texture_unit);

	~Texture();

private:
	std::string m_filename;
	GLenum m_texture_target;
	unsigned char* data;
	GLenum m_texture_obj;
};


class CubemapTexture {
public:
	CubemapTexture(
		const std::string& pox_x_filepath,
		const std::string& pox_y_filepath,
		const std::string& pox_z_filepath,
		const std::string& neg_x_filepath,
		const std::string& neg_y_filepath,
		const std::string& neg_z_filepath
		);
	~CubemapTexture();
	bool load();
	void bind(GLenum texture_unit);
private:
	std::string m_filepaths[6];
	GLuint m_texture_obj;
};

#endif // !_TEXTURE_H