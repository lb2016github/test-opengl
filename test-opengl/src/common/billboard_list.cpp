#include "billboard_list.h"


BillboardList::BillboardList() {
	m_vbo = 0;
	m_tex = NULL;
	m_tech = NULL;
}
bool BillboardList::init(const std::string& tex_filename, float width, float height) {
	glGenBuffers(1, &m_vbo);
	m_tex = new Texture(GL_TEXTURE_2D, tex_filename);
	m_tex->load();

	m_tech = new BillboardTechnique();
	m_tech->init();

	m_width = width;
	m_height = height;
	return true;
}
void BillboardList::render(const std::vector<M3DVector3f>& pos_list, const M3DVector3f& cam_pos, const M3DMatrix44f& vp) {
	m_tex->bind(GL_TEXTURE0);

	m_tech->enable();
	m_tech->set_color_map_tex_unit(0);
	m_tech->set_billboard_size(m_width, m_height);
	m_tech->set_camera_position(cam_pos);
	m_tech->set_vp_trans(vp);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, pos_list.size() * sizeof(M3DVector3f), &pos_list[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(M3DVector3f), (GLvoid*)0);
	glDrawArrays(GL_POINTS, 0, pos_list.size());
	glDisableVertexAttribArray(0);
}