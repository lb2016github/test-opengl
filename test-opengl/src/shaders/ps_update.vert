#version 330
layout(location = 0) in float type;
layout(location = 1) in float age;
layout(location = 2) in vec3 position;
layout(location = 3) in vec3 velocity;

struct Point{
	float type;
	float age;
	vec3 position;
	vec3 velocity;
};

out Point vs_out;

void main(){
	vs_out.type = type;
	vs_out.age = age;
	vs_out.position = position;
	vs_out.velocity = velocity;
}