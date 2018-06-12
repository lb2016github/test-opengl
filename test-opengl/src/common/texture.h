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

class RandomTexture
{
public:
	RandomTexture();
	~RandomTexture();

	void bind(GLenum texture_unit);
	bool init(unsigned int size);

private:
	GLuint m_texture_obj;
};

class PickingTexture {
public:
	PickingTexture();
	~PickingTexture();
	bool init(unsigned int width, unsigned int height);
	void enable_writing();
	void disable_writing();
	void bind(GLenum tex_unit);

	struct PixelInfo {
		float obj_id;	// 标识哪个Mesh
		float mesh_id;	// 标识子mesh
		float prim_id;	// 标识图元
		PixelInfo(): obj_id(0.0), mesh_id(0.0), prim_id(0.0){}
	};
	PixelInfo read_pixel(unsigned int x, unsigned int y);
private:
	GLuint m_fbo;
	GLuint m_picking_texture;
	GLuint m_depth_texture;
};

#endif // !_TEXTURE_H