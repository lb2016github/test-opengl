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
	M3DVector3f m_pos;
	M3DVector2f m_coor;
	M3DVector3f m_normal;
	M3DVector3f m_tangent;

public:
	Vertex(float x, float y, float z, float u, float v);
	Vertex(float x, float y, float z, float u, float v, float n_x, float n_y, float n_z);
	Vertex(float x, float y, float z, float u, float v, float n_x, float n_y, float n_z, float t_x, float t_y, float t_z);

private:
	//Vertex(const Vertex& ver);
	//Vertex& operator=(const Vertex& ver);

	void transform(M3DMatrix44f trans);
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

class IMesh {
public:
	virtual bool load_mesh(const std::string& filename)=0;
	virtual void render(IRenderCallback* callback, GLenum mode=GL_TRIANGLES) = 0;
	virtual void render_primitive(unsigned int mesh_id, unsigned int primitive_id) {};
};

class SimpleMesh: public IMesh {
public:
	virtual bool load_mesh(const std::string& filename);
	void render(IRenderCallback* callback, GLenum mode = GL_TRIANGLES);

protected:
	void calc_normal(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	MeshEntity m_mesh_ent;
	Texture* m_tex;
};

class Mesh : public IMesh {
public:
	Mesh();
	~Mesh();

	virtual bool load_mesh(const std::string& filename);
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

	virtual bool load_mesh(const std::string& filename);
	virtual void render(IRenderCallback* callback, GLenum mode = GL_TRIANGLES);
	virtual void render_primitive(unsigned int mesh_id, unsigned int primitive_id);
private:
	bool init_from_scene(const aiScene* scene, const std::string& filename);
	bool init_materials(const aiScene* scene, const std::string& filename);
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
};

#endif // _MESH_H
