#version 410
layout(triangles_adjacency) in;
layout(line_strip, max_vertices=6) out;

in vec3 world_pos[];

uniform vec3 g_light_pos;

void emit_vertices(int start_index, int end_index){
	gl_Position = gl_in[start_index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[end_index].gl_Position;
	EmitVertex();
	EndPrimitive();
}

void main(){
	vec3 e1 = world_pos[2] - world_pos[0];
	vec3 e2 = world_pos[4] - world_pos[2];
	vec3 e3 = world_pos[0] - world_pos[4];
	vec3 e4 = world_pos[1] - world_pos[0];
	vec3 e5 = world_pos[3] - world_pos[2];
	vec3 e6 = world_pos[5] - world_pos[4];

	vec3 face_normal = cross(e3, e1);
	vec3 light_dir = g_light_pos - world_pos[0];

	if(dot(light_dir, face_normal) > 0.00001){
		vec3 tmp_normal = cross(e4, e1);
		if(dot(light_dir, tmp_normal) <= 0){
			emit_vertices(0, 2);
		}

		light_dir = g_light_pos - world_pos[2];
		tmp_normal = cross(e5, e2);
		if(dot(light_dir, tmp_normal) <= 0){
			emit_vertices(2, 4);
		}

		light_dir = g_light_pos - world_pos[4];
		tmp_normal = cross(e6, e3);
		if(dot(light_dir, tmp_normal) <= 0){
			emit_vertices(4, 0);
		}
	}
}