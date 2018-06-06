#version 330
layout(points) in;
layout(triangle_strip, max_vertices=4) out;

out vec2 tex_coord;
uniform vec3 cam_pos;		// 相机坐标
uniform float width;		// 宽度
uniform float height;		// 高度

void main(){
	vec3 dir2cam = normalize(cam_pos - gl_in[0].gl_Position.xyz);
	vec3 up = vec3(0, 1, 0);
	vec3 right = cross(up, dir2cam);

	// generate left down corner
	vec4 pos = gl_in[0].gl_Position - vec4(right * (-width / 2), 0);
	gl_Position = pos;
	tex_coord = vec2(0, 0);
	EmitVertex();

	// generate left up corner
	pos += vec4(up * height, 0);
	gl_Position = pos;
	tex_coord = vec2(0, 1);
	EmitVertex();

		// generate right down corner
	pos -= vec4(up * (-height), 0);
	pos += vec4(right * width, 0);
	gl_Position = pos;
	tex_coord = vec2(1, 0);
	EmitVertex();

	// generate right up corner
	pos += vec4(up * height, 0);
	gl_Position = pos;
	tex_coord = vec2(1, 1);
	EmitVertex();

	EndPrimitive();
	
}