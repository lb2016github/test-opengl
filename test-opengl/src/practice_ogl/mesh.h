#ifndef _MESH_H
#define _MESH_H
#include "glad\glad.h"
#include "math3d\math3d.h"
#include <vector>
#include "texture.h"

class Vertex {
public:
	M3DVector3f m_pos;
	M3DVector2f m_coor;
	M3DVector3f m_normal;

public:
	Vertex(float x, float y, float z, float u, float v);

private:
	//Vertex(const Vertex& ver);
	//Vertex& operator=(const Vertex& ver);

	void transform(M3DMatrix44f trans);
};


class MeshEntity {
public:
	void init(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

public:
	GLuint vb;
	GLuint ib;
	unsigned int num_indices;
};

class IMesh {
public:
	virtual bool load_mesh()=0;
	virtual void render()=0;
};

class SimpleMesh: public IMesh {
public:
	virtual bool load_mesh();
	void render();

protected:
	void calc_normal(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);

	MeshEntity m_mesh_ent;
	Texture* m_tex;
};

#endif // _MESH_H
