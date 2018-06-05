#include "mesh.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "const.h"

Vertex::Vertex(float x, float y, float z, float u, float v) {
	m3dLoadVector3(m_pos, x, y, z);
	m3dLoadVector2(m_coor, u, v);
	m3dLoadVector3(m_normal, 0, 0, 0);
	m3dLoadVector3(m_tangent, 0, 0, 0);
}

Vertex::Vertex(float x, float y, float z, float u, float v, float n_x, float n_y, float n_z) {
	m3dLoadVector3(m_pos, x, y, z);
	m3dLoadVector2(m_coor, u, v);
	m3dLoadVector3(m_normal, n_x, n_y, n_z);
	m3dLoadVector3(m_tangent, 0, 0, 0);
}

Vertex::Vertex(float x, float y, float z, float u, float v, float n_x, float n_y, float n_z, float t_x, float t_y, float t_z) {
	m3dLoadVector3(m_pos, x, y, z);
	m3dLoadVector2(m_coor, u, v);
	m3dLoadVector3(m_normal, n_x, n_y, n_z);
	m3dLoadVector3(m_tangent, t_x, t_y, t_z);
}

void Vertex::transform(M3DMatrix44f trans) {
	M3DVector3f tmp;
	m3dCopyVector3(tmp, m_pos);
	m3dTransformVector3(m_pos, tmp, trans);
}

MeshEntity::MeshEntity() {
	vb = INVALID_OGL_VALUE;
	ib = INVALID_OGL_VALUE;
	num_indices = 0;
	material_index = INVALID_MATERIAL;
}

MeshEntity::~MeshEntity() {
	if (vb != INVALID_OGL_VALUE) {
		glDeleteBuffers(1, &vb);
	}
	if (ib != INVALID_OGL_VALUE) {
		glDeleteBuffers(1, &ib);
	}
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


bool SimpleMesh::load_mesh(const std::string& filename) {
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

void SimpleMesh::calc_normal(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
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

void SimpleMesh::render() {
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

/********************************************
Mesh implementation
*********************************************/
Mesh::Mesh() {

}
Mesh::~Mesh() {
	clear();
}

void Mesh::render() {
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	
	for (int i = 0; i < m_entities.size(); ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, m_entities[i].vb);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_entities[i].ib);
		
		const unsigned int material_index = m_entities[i].material_index;
		if (material_index < m_textures.size() && m_textures[material_index]) {
			m_textures[material_index]->bind(GL_TEXTURE0);
		}

		glDrawElements(GL_TRIANGLES, m_entities[i].num_indices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

bool Mesh::load_mesh(const std::string& filename) {
	clear();

	bool ret = false;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (scene) {
		ret = init_from_scene(scene, filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", filename.c_str(), importer.GetErrorString());
	}
	return ret;
}

bool Mesh::init_from_scene(const aiScene* scene, const std::string& filename) {
	m_entities.resize(scene->mNumMeshes);
	m_textures.resize(scene->mNumMaterials);

	// init meshes in the scene one by one
	for (unsigned int i = 0; i < m_entities.size(); ++i) {
		const aiMesh* ai_mesh = scene->mMeshes[i];
		init_mesh(i, ai_mesh);
	}

	return init_materials(scene, filename);
}

void Mesh::init_mesh(unsigned int index, const aiMesh* ai_mesh){
	m_entities[index].material_index = ai_mesh->mMaterialIndex;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	aiVector3D default_tangent(1, 0, 0);

	// init vertices
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < ai_mesh->mNumVertices; ++i) {
		const aiVector3D* pos = &(ai_mesh->mVertices[i]);
		const aiVector3D* normal = &(ai_mesh->mNormals[i]);
		const aiVector3D* cood = ai_mesh->HasTextureCoords(0) ? &(ai_mesh->mTextureCoords[0][i]): &Zero3D;
		const aiVector3D* tangent = &default_tangent;
		if (ai_mesh->mTangents) {
			tangent = &(ai_mesh->mTangents[i]);
		}
		
		Vertex v(pos->x, pos->y, pos->z, cood->x, cood->y, normal->x, normal->y, normal->z, tangent->x, tangent->y, tangent->z);
		vertices.push_back(v);
	}

	// init indicies
	for (int i = 0; i < ai_mesh->mNumFaces; ++i) {
		const aiFace& face = ai_mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	// init MeshEntry
	m_entities[index].init(vertices, indices);
}

bool Mesh::init_materials(const aiScene* scene, const std::string& filename) {
	bool ret = true;

	// extract the directory of file
	std::string::size_type slash_index = filename.find_last_of("/");
	std::string dir;
	if (slash_index == std::string::npos) {
		dir = ".";
	}
	else if (slash_index == 0) {
		dir = "/";
	}
	else {
		dir = filename.substr(0, slash_index);
	}

	// extract filepath of textures and generate textures
	for (int i = 0; i < scene->mNumMaterials; ++i) {
		const aiMaterial* material = scene->mMaterials[i];
		m_textures[i] = NULL;
		if (material->GetTextureCount(aiTextureType_DIFFUSE) <= 0){
			continue;
		}
		aiString path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			std::string full_path = dir + "/" + path.data;
			m_textures[i] = new Texture(GL_TEXTURE_2D, full_path.c_str());

			if (!m_textures[i]->load()) {
				printf("Error: loading texture '%s'\n", full_path.c_str());
				delete m_textures[i];
				m_textures[i] = NULL;
				ret = false;
			}
			else {
				printf("Loaded texture '%s'\n", full_path.c_str());
			}
		}

		// load white texture for null textures
		if (!m_textures[i]) {
			m_textures[i] = new Texture(GL_TEXTURE_2D, "res/white.png");
			ret = m_textures[i]->load();
		}
	}
	return ret;

}

void Mesh::clear() {
	for (int i = 0; i < m_textures.size(); ++i) {
		if (m_textures[i]) {
			delete m_textures[i];
		}
	}
}