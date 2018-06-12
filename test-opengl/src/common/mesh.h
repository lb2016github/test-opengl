#ifndef _MESH_H
#define _MESH_H
#include "glad\glad.h"
#include "math3d\math3d.h"
#include <vector>
#include "texture.h"
#include <string>
#include "assimp/scene.h"
#include "i_callbacks.h"

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
	virtual void render(IRenderCallback* callback) = 0;
	virtual void render(unsigned int mesh_id, unsigned int primitive_id) {};
};

class SimpleMesh: public IMesh {
public:
	virtual bool load_mesh(const std::string& filename);
	void render(IRenderCallback* callback);

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
	virtual void render(IRenderCallback* callback);
	virtual void render(unsigned int mesh_id, unsigned int primitive_id);
private:
	bool init_from_scene(const aiScene* scene, const std::string& filename);
	void init_mesh(unsigned int index, const aiMesh* ai_mesh);
	bool init_materials(const aiScene* scene, const std::string& filename);
	void clear();

	std::vector<MeshEntity> m_entities;
	std::vector<Texture*> m_textures;

};

#endif // _MESH_H
