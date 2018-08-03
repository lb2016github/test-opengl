#ifndef _MESH_H
#define _MESH_H
#include "glad\glad.h"
#include "math3d\math3d.h"
#include <vector>
#include "texture.h"
#include <string>
#include "assimp/scene.h"
#include "i_callbacks.h"
#include "const.h"
#include "math3d/math3d_ex.h"
#include <map>

#define INDEX_BUFFER_INDEX 0
#define POSITION_BUFFER_INDEX 1
#define TEX_COORD_BUFFER_INDEX 2
#define NORMAL_BUFFER_INDEX 3
#define TANGENT_BUFFER_INDEX 4
#define WVP_BUFFER_INDEX 5
#define WORLD_BUFFER_INDEX 6
#define BUFFER_SIZE 7


class Vertex {
public:
	Vector3 m_pos;
	Vector2 m_coor;
	Vector3 m_normal;
	Vector3 m_tangent;

public:
	Vertex(float x, float y, float z, float u, float v);
	Vertex(float x, float y, float z, float u, float v, float n_x, float n_y, float n_z);
	Vertex(float x, float y, float z, float u, float v, float n_x, float n_y, float n_z, float t_x, float t_y, float t_z);

private:
	//Vertex(const Vertex& ver);
	//Vertex& operator=(const Vertex& ver);

	void transform(Matrix& trans);
};


class MeshEntity {
public:
	MeshEntity();
	~MeshEntity();

	void init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

public:
	GLuint vb;
	GLuint ib;
	unsigned int num_indices;
	unsigned int material_index;
};

//***********************************
// 实现Mesh的ajdacency
//***********************************
struct Edge {
	Edge(unsigned int i1, unsigned int i2 = -1);
	unsigned int idx_1;
	unsigned int idx_2;
};
struct CompareEdges {
	bool operator()(const Edge& e1, const Edge& e2) const;
};
struct CompareVectors {
	bool operator()(const aiVector3D& a, const aiVector3D& b) const;
};
struct Neighbors {
	Neighbors() : n_idx_1(-1), n_idx_2(-1) {}
	unsigned int find_other(unsigned int idx) { return idx == n_idx_1 ? n_idx_2 : n_idx_1; }
	void add_neighbor(unsigned int i);
	int n_idx_1;
	int n_idx_2;
};
class MeshAdjacentHelper {
public:
	std::vector<unsigned int> create_adjacent_indices(const aiMesh& ai_mesh);
};

class IMesh {
public:
	virtual bool load_mesh(const std::string& filename, bool with_adjacencies = false)=0;
	virtual void render(IRenderCallback* callback, GLenum mode=GL_TRIANGLES) = 0;
	virtual void render_primitive(unsigned int mesh_id, unsigned int primitive_id) {}
	virtual void render_instances(IRenderCallback* callback, GLenum mode, unsigned int num_instance, const Matrix* wvp, const Matrix* world) {}
};

class Mesh : public IMesh {
public:
	Mesh();
	~Mesh();

	virtual bool load_mesh(const std::string& filename, bool with_adjacencies=false);
	virtual void render(IRenderCallback* callback, GLenum mode = GL_TRIANGLES);
	virtual void render_primitive(unsigned int mesh_id, unsigned int primitive_id);
private:
	bool init_from_scene(const aiScene* scene, const std::string& filename);
	void init_mesh(unsigned int index, const aiMesh* ai_mesh);
	bool init_materials(const aiScene* scene, const std::string& filename);
	void clear();

	std::vector<MeshEntity> m_entities;
	std::vector<Texture*> m_textures;

};

class VAOMesh : public IMesh {
public:
	VAOMesh();
	~VAOMesh();

	virtual bool load_mesh(const std::string& filename, bool with_adjacencies = false);
	virtual void render(IRenderCallback* callback, GLenum mode = GL_TRIANGLES);
	virtual void render_primitive(unsigned int mesh_id, unsigned int primitive_id);
	virtual void render_instances(IRenderCallback* callback, GLenum mode, unsigned int num_instance, const Matrix* wvp, const Matrix* world);
private:
	bool init_from_scene(const aiScene* scene, const std::string& filename);
	bool init_materials(const aiScene* scene, const std::string& filename);
	std::vector<unsigned int> create_adjacent_indices(const aiMesh& ai_mesh);
	void clear();

private:
	struct MeshEntity {
		int num_indices;
		int base_vertices;
		int base_index;
		int material_index;
		MeshEntity() : num_indices(0), base_vertices(0), base_index(0), material_index(0){}
	};

	std::vector<MeshEntity> m_entities;
	std::vector<Texture*> m_textures;

	GLuint m_vao;
	GLuint m_buffers[BUFFER_SIZE];	// index, pos, tex_coord, normal, tangent, wvp, world

	// adjacence相关
	bool m_with_adjacencies;
};


#endif // _MESH_H
