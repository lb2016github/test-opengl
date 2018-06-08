#ifndef _BILLBOARD_H
#define _BILLBOARD_H
#include "glad/glad.h"
#include "texture.h"
#include <string>
#include <vector>
#include "math3d/math3d.h"
#include "technique.h"

class BillboardList {
public:
	BillboardList();
	bool init(const std::string& tex_filename, float width, float height);
	void render(const std::vector<M3DVector3f>& pos_list, const M3DVector3f& cam_pos, const M3DMatrix44f& vp);

private:
	GLuint m_vbo;
	Texture* m_tex;
	BillboardTechnique* m_tech;
	float m_width;
	float m_height;
};

#endif // !_BILLBOARD_H
