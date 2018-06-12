#version 330

out vec3 frag_color;

uniform int g_obj_id;
uniform int g_mesh_id;

void main(){
	frag_color = vec3(g_obj_id, g_mesh_id, gl_PrimitiveID + 1);
}