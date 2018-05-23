#include "mesh.h"

Vertex::Vertex(float x, float y, float z, float u, float v) {
	m_pos[0] = x, m_pos[1] = y, m_pos[2] = z;
	m_coor[0] = u, m_coor[1] = v;
	m3dLoadVector3(m_normal, 0, 0, 0);
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, &indices[0], GL_STATIC_DRAW);
}


bool Mesh::load_mesh() {
	// init vertexes
	std::vector<Vertex> vertices = {
		Vertex(-1.0f, -1.0f, 0.5773f, 0, 0),
		Vertex(0.0f, -1.0f, -1.15475f, 0.5f, 0),
		Vertex(1.0f, -1.0f, 0.5773f, 1, 0),
		Vertex(0.0f, 1.0f, 0.0f, 0.5f, 1)
	};
	std::vector<unsigned int> indices = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		1, 2, 0
	};

	calc_normal(vertices, indices);

	m_mesh_ent.init(vertices, indices);
	
	// init texture
	m_tex = new Texture(GL_TEXTURE_2D, "res/test.png");
	m_tex->load();

	return true;
}

void Mesh::calc_normal(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
	for (int i = 2; i < indices.size(); i += 3) {
		unsigned int idx_1 = indices[i - 2], idx_2 = indices[i - 1], idx_3 = indices[i];
		M3DVector3f line_1, line_2, normal;
		m3dSubtractVectors3(line_1, vertices[idx_2].m_pos, vertices[idx_1].m_pos);
		m3dSubtractVectors3(line_2, vertices[idx_3].m_pos, vertices[idx_1].m_pos);
		m3dCrossProduct3(normal, line_1, line_2);
		m3dNormalizeVector3(normal);

		// add normal to vertex
		m3dAddVectors3(vertices[idx_1].m_normal, vertices[idx_1].m_normal, normal);
		m3dAddVectors3(vertices[idx_2].m_normal, vertices[idx_2].m_normal, normal);
		m3dAddVectors3(vertices[idx_3].m_normal, vertices[idx_3].m_normal, normal);
	}

	// average normal
	for (int i = 0; i < vertices.size(); ++i) {
		m3dNormalizeVector3(vertices[i].m_normal);
	}
}

void Mesh::render() {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
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
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(const GLvoid*)(sizeof(float) * 5)
	);
	m_tex->bind(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, m_mesh_ent.num_indices, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}