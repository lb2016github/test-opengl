#ifndef _TECHNIQUE_H
#define _TECHNIQUE_H
#include "glad\glad.h"
#include <list>


class Technique {
public:
	Technique();
	virtual ~Technique();
	virtual bool init();
	void enable();

protected:
	bool add_shader(GLenum shader_type, const char* filename);	// add shader object
	bool finalize();	// compile and link shader
	GLuint m_program_id;

private:
	typedef std::list<GLuint> ShaderObjList;
	ShaderObjList m_shader_obj_list;

};

#endif // !_TECHNIQUE_H
