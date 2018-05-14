#include <string>
#include "glad\glad.h"


class Texture {
public:
	Texture(GLenum texture_target, char* filename);

	bool load();

	void bind(GLenum texture_unit);

	~Texture();

private:
	char* m_filename;
	GLenum m_texture_target;
	unsigned char* data;
	GLenum m_texture_obj;
};