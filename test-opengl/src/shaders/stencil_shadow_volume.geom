#version 410
layout(triangles_adjacency) in;
layout(triangle_strip, max_vertices=18) out;

struct VSOut{
	vec3 w_position;
};

in VSOut vs_out[];

uniform vec3 g_light_position;
uniform mat4 g_vp;

#define EPSILON 0.0001

void emit_quad(int start_idx, int end_idx){
	vec3 light_dir = normalize(vs_out[start_idx].w_position - g_light_position);
	gl_Position = g_vp * vec4(vs_out[start_idx].w_position + light_dir * EPSILON, 1);
	EmitVertex();
	gl_Position = g_vp * vec4(light_dir, 0);
	EmitVertex();
	light_dir = normalize(vs_out[end_idx].w_position - g_light_position);
	gl_Position = g_vp * vec4(vs_out[end_idx].w_position + light_dir * EPSILON, 1);
	EmitVertex();
	gl_Position = g_vp * vec4(light_dir, 0);
	EmitVertex();
	EndPrimitive();

}

void main(){
	vec3 e1 = vs_out[2].w_position - vs_out[0].w_position;
	vec3 e2 = vs_out[4].w_position - vs_out[2].w_position;
	vec3 e3 = vs_out[0].w_position - vs_out[4].w_position;
	vec3 e4 = vs_out[1].w_position - vs_out[0].w_position;
	vec3 e5 = vs_out[3].w_position - vs_out[2].w_position;
	vec3 e6 = vs_out[5].w_position - vs_out[4].w_position;

	vec3 norm = cross(e3, e1);
	vec3 light_dir = g_light_position - vs_out[0].w_position;
	if(dot(light_dir, norm) > 0.00001){
		// emit quads
		norm = cross(e4, e1);
		if(dot(light_dir, norm) <= 0){
			emit_quad(0, 2);
		}

		light_dir = g_light_position - vs_out[2].w_position;
		norm = cross(e5, e2);
		if(dot(light_dir, norm) <= 0){
			emit_quad(2, 4);
		}

		light_dir = g_light_position - vs_out[4].w_position;
		norm = cross(e6, e3);
		if(dot(light_dir, norm) <= 0){
			emit_quad(4, 0);
		}
		// emit front cap
		light_dir = normalize(vs_out[0].w_position - g_light_position);
		gl_Position = g_vp * vec4(vs_out[0].w_position + light_dir * EPSILON, 1);
		EmitVertex();
		light_dir = normalize(vs_out[2].w_position - g_light_position);
		gl_Position = g_vp * vec4(vs_out[2].w_position + light_dir * EPSILON, 1);
		EmitVertex();
		light_dir = normalize(vs_out[4].w_position - g_light_position);
		gl_Position = g_vp * vec4(vs_out[4].w_position + light_dir * EPSILON, 1);
		EmitVertex();
		EndPrimitive();
		// emit back cap
		light_dir = normalize(vs_out[0].w_position - g_light_position);
		gl_Position = g_vp * vec4(light_dir, 0);
		EmitVertex();
		light_dir = normalize(vs_out[4].w_position - g_light_position);
		gl_Position = g_vp * vec4(light_dir, 0);
		EmitVertex();
		light_dir = normalize(vs_out[2].w_position - g_light_position);
		gl_Position = g_vp * vec4(light_dir, 0);
		EmitVertex();
		EndPrimitive();

	}
}