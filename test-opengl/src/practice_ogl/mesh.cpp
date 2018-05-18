#include "mesh.h"

Vertex::Vertex(float x, float y, float z, float u, float v) {
	m_pos[0] = x, m_pos[1] = y, m_pos[2] = z;
	m_coor[0] = u, m_coor[1] = v;
}

void Vertex::transform(M3DMatrix44f trans) {
	M3DVector3f tmp;
	m3dCopyVector3(tmp, m_pos);
	m3dTransformVector3(m_pos, tmp, trans);
}

void MeshEntity::init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
	num_indices = indices.size();

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * num_indices, &indices[0], GL_STATIC_DRAW);
}


bool Mesh::load_mesh() {
	// init vertexes
	std::vector<Vertex> vertices = {
		Vertex(-1.0f, -1.0f, 0.5773f, 0, 0),
		Vertex(0.0f, -1.0f, -1.15475f, 0.5f, 0),
		Vertex(1.0f, -1.0f, 0.5773f, 1, 0),
		Vertex(0.0f, 1.0f, 0.0f, 0.5f, 1)
	};
	m_vertex.assign(vertices.begin(), vertices.end());
	std::vector<unsigned int> indices = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		1, 2, 0
	};
	m_mesh_ent.init(vertices, indices);
	
	// init texture
	m_tex = new Texture(GL_TEXTURE_2D, "res/test.png");
	m_tex->load();

	return true;
}
void Mesh::render() {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)0
	);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(const GLvoid*)(sizeof(float) * 3)
	);
	m_tex->bind(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, m_mesh_ent.num_indices, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Mesh::transform(M3DMatrix44f trans) {
	for (std::vector<Vertex>::iterator iter = m_vertex.begin(); iter != m_vertex.end(); ++iter) {
		M3DVector4f ori, end;
		ori[0] = iter->m_pos[0], ori[1] = iter->m_pos[1], ori[2] = iter->m_pos[2], ori[3] = 1;
		m3dTransformVector4(end, ori, trans);
		printf("(%f, %f, %f) -> (%f, %f, %f)\n", ori[0], ori[1], ori[2], end[0], end[1], end[2]);
	}
}