#include "mesh.h"
#include "assimp/postprocess.h"
#include "assimp/Importer.hpp"
#include "const.h"
#include "math3d/math3d_ex.h"
#include "utils.h"

Vertex::Vertex(float x, float y, float z, float u, float v)
{
	Vertex(x, y, z, u, v, 0, 0, 1, 0, 0, 1);
}

Vertex::Vertex(float x, float y, float z, float u, float v, float n_x, float n_y, float n_z){
	Vertex(x, y, z, u, v, n_x, n_y, n_z, 0, 0, 1);
}

Vertex::Vertex(float x, float y, float z, float u, float v, float n_x, float n_y, float n_z, float t_x, float t_y, float t_z):
	m_pos(Vector3(x, y, z)), m_coor(Vector2(u, v)), m_normal(Vector3(n_x, n_y, n_z)), m_tangent(Vector3(t_x, t_y, t_z)) {
}

void Vertex::transform(Matrix& trans) {
	m_pos = trans * m_pos;
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

/********************************************
Mesh implementation
*********************************************/
Mesh::Mesh() {

}
Mesh::~Mesh() {
	clear();
}

void Mesh::render(IRenderCallback* callback, GLenum mode) {
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
		if (callback) {
			callback->on_draw_start_callback(i);
		}
		glDrawElements(mode, m_entities[i].num_indices, GL_UNSIGNED_INT, 0);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
}

void Mesh::render_primitive(unsigned int mesh_id, unsigned int primitive_id) {
	assert(mesh_id < m_entities.size());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, m_entities[mesh_id].vb);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)32);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_entities[mesh_id].ib);

	const unsigned int material_index = m_entities[mesh_id].material_index;
	if (material_index < m_textures.size() && m_textures[material_index]) {
		m_textures[material_index]->bind(GL_TEXTURE0);
	}

	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(primitive_id * sizeof(GLuint) * 3));

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



/********************************************
VAOMesh implementation
*********************************************/
VAOMesh::VAOMesh() {

}
VAOMesh::~VAOMesh() {
	clear();
}

void VAOMesh::render(IRenderCallback* callback, GLenum mode) {
	glBindVertexArray(m_vao);

	for (int i = 0; i < m_entities.size(); ++i) {
		const unsigned int material_index = m_entities[i].material_index;
		if (material_index < m_textures.size() && m_textures[material_index]) {
			m_textures[material_index]->bind(GL_TEXTURE0);
		}
		if (callback) {
			callback->on_draw_start_callback(i);
		}
		glDrawElementsBaseVertex(
			mode, m_entities[i].num_indices, GL_UNSIGNED_INT,
			(GLvoid*)(sizeof(unsigned int) * m_entities[i].base_index),
			m_entities[i].base_vertices
			);
	}
	glBindVertexArray(0);
}

void VAOMesh::render_primitive(unsigned int mesh_id, unsigned int primitive_id) {
	assert(mesh_id < m_entities.size());
	glBindVertexArray(m_vao);
	const unsigned int material_index = m_entities[mesh_id].material_index;
	if (material_index < m_textures.size() && m_textures[material_index]) {
		m_textures[material_index]->bind(GL_TEXTURE0);
	}

	glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GL_UNSIGNED_INT) * (m_entities[mesh_id].base_index + (primitive_id * 3))), m_entities[mesh_id].base_vertices);
	glBindVertexArray(0);
}

void VAOMesh::render_instances(IRenderCallback* callback, GLenum mode, unsigned int num_instance, const Matrix* wvp, const Matrix* world) {
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WVP_BUFFER_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix) * num_instance, wvp, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WORLD_BUFFER_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Matrix) * num_instance, world, GL_DYNAMIC_DRAW);
	glBindVertexArray(m_vao);
	for (int i = 0; i < m_entities.size(); ++i) {
		const unsigned int material_index = m_entities[i].material_index;
		if (material_index < m_textures.size() && m_textures[material_index]) {
			m_textures[material_index]->bind(GL_TEXTURE0);
		}
		if (callback) {
			callback->on_draw_start_callback(i);
		}
		glDrawElementsInstancedBaseVertex(
			mode, m_entities[i].num_indices, GL_UNSIGNED_INT, (GLvoid*)(m_entities[i].base_index * sizeof(unsigned int)), num_instance, m_entities[i].base_vertices
		);
	}

	glBindVertexArray(0);

}

bool VAOMesh::load_mesh(const std::string& filename) {
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

bool VAOMesh::init_from_scene(const aiScene* scene, const std::string& filename) {
	m_entities.resize(scene->mNumMeshes);
	m_textures.resize(scene->mNumMaterials);

	std::vector<Vector3> positions, normals, tangents;
	std::vector<Vector2> tex_coords;
	std::vector<unsigned int> indices;
	unsigned int num_vertices = 0, num_indices = 0;
	// init meshes in the scene one by one
	aiVector3D default_tangent(1, 0, 0);
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	for (unsigned int i = 0; i < m_entities.size(); ++i) {
		const aiMesh* ai_mesh = scene->mMeshes[i];
		m_entities[i].base_index = num_indices;
		m_entities[i].base_vertices = num_vertices;
		m_entities[i].material_index = ai_mesh->mMaterialIndex;
		for (unsigned int i = 0; i < ai_mesh->mNumVertices; ++i) {
			// read data
			const aiVector3D* pos = &(ai_mesh->mVertices[i]);
			const aiVector3D* normal = &(ai_mesh->mNormals[i]);
			const aiVector3D* cood = ai_mesh->HasTextureCoords(0) ? &(ai_mesh->mTextureCoords[0][i]) : &Zero3D;
			const aiVector3D* tangent = &default_tangent;
			// save data
			positions.push_back(Vector3(pos->x, pos->y, pos->z));
			normals.push_back(Vector3(normal->x, normal->y, normal->z));
			tex_coords.push_back(Vector2(cood->x, cood->y));
			tangents.push_back(Vector3(tangent->x, tangent->y, tangent->z));
			// add vertex
			num_vertices += 1;
		}
		for (unsigned int i = 0; i < ai_mesh->mNumFaces; ++i) {
			// read data
			const aiFace& face = ai_mesh->mFaces[i];
			assert(face.mNumIndices == 3);
			// save data
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
			// add index
			num_indices += 3;
		}
		m_entities[i].num_indices = num_indices - m_entities[i].base_index;
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(BUFFER_SIZE, m_buffers);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER_INDEX]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POSITION_BUFFER_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions[0]), &positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(LOCATION_POSITON_INDEX);
	glVertexAttribPointer(LOCATION_POSITON_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TEX_COORD_BUFFER_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, tex_coords.size() * sizeof(tex_coords[0]), &tex_coords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(LOCATION_TEX_COORD_INDEX);
	glVertexAttribPointer(LOCATION_TEX_COORD_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[NORMAL_BUFFER_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(LOCATION_NORMAL_INDEX);
	glVertexAttribPointer(LOCATION_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[TANGENT_BUFFER_INDEX]);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(tangents[0]), &tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(LOCATION_TANGENT_INDEX);
	glVertexAttribPointer(LOCATION_TANGENT_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WVP_BUFFER_INDEX]);
	for (int i = 0; i < 4; ++i)
	{
		glEnableVertexAttribArray(LOCATION_WVP_INDEX + i);
		glVertexAttribPointer(LOCATION_WVP_INDEX + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(sizeof(float) * i * 4));
		glVertexAttribDivisor(LOCATION_WVP_INDEX + i, 1);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[WORLD_BUFFER_INDEX]);
	for (int i = 0; i < 4; ++i)
	{
		glEnableVertexAttribArray(LOCATION_WORLD_INDEX + i);
		glVertexAttribPointer(LOCATION_WORLD_INDEX + i, 4, GL_FLOAT, GL_FALSE, sizeof(Matrix), (GLvoid*)(sizeof(float) * i * 4));
		glVertexAttribDivisor(LOCATION_WORLD_INDEX + i, 1);
	}

	glBindVertexArray(0);

	bool rst = glGetError() == GL_NO_ERROR;
	
	return init_materials(scene, filename) && rst;
}

bool VAOMesh::init_materials(const aiScene* scene, const std::string& filename) {
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
		if (material->GetTextureCount(aiTextureType_DIFFUSE) <= 0) {
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

void VAOMesh::clear() {
	for (int i = 0; i < m_textures.size(); ++i) {
		if (m_textures[i]) {
			delete m_textures[i];
		}
	}
	for (int i = 0; i < BUFFER_SIZE; ++i) {
		if (m_buffers[i]) glDeleteBuffers(1, &(m_buffers[i]));
	}
	glDeleteVertexArrays(1, &m_vao);
}