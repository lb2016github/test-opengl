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

#endif // !_TEXTURE_H