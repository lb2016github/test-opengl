#version 410 core
#define MAX_LIGHT_NUM 2
#define PI 3.1415926

struct BaseLight{
	vec3 color;
	float ambiant_intensity;
	float diffuse_intensity;
};

// ˥������
struct Attenuation{
	float constant;
	float linear;
	float expo;
};

struct PointLight{
	BaseLight base_light;
	vec3 position;
	Attenuation atten;
};

struct VSOut{
	vec2 coord;
	vec3 w_pos;
	vec3 w_normal;
};

uniform sampler2D g_tex_roughness;	// �ֲڶ���ͼ
uniform sampler2D g_tex_metalness;	// ��������ͼ
uniform sampler2D g_tex_albedo;	// ������������ͼ����diffuse��ֻ�Ǹ����ϲ�ͬ��
uniform sampler2D g_tex_ao;		// ao��ͼ

uniform PointLight g_point_lights[MAX_LIGHT_NUM];	// ���Դ
uniform int g_light_num;		// ������Ŀ
uniform vec3 g_cam_pos;			// ���λ��

in VSOut vs_out;			// ����
out vec4 g_frag_color;	// �����ɫ

float distribution_ggx(vec3 n, vec3 h, float roughness);
vec3 fresnel_schlick(float h_o_v, vec3 f0);
float geometry_schlick_ggx(float costheta, float roughness);
float geometry_smith(vec3 n, vec3 l, vec3 v, float roughness);
float calc_attenuation(Attenuation atten, float dis);	// �������˥��

void main(){
	vec3 w_normal = normalize(vs_out.w_normal);	// ����normal����
	vec3 w_pos = vs_out.w_pos;
	float roughness = texture(g_tex_roughness, vs_out.coord).x;
	float metalness = texture(g_tex_metalness, vs_out.coord).x;
	vec3 albedo = texture(g_tex_albedo, vs_out.coord).xyz;
	vec3 ao = texture(g_tex_ao, vs_out.coord).xyz;
	vec3 view_dir = normalize(g_cam_pos - w_pos);	// �������

	vec3 f0 = vec3(0.04);
	f0 = mix(f0, albedo, metalness);

	vec3 color = vec3(0, 0, 0);
	float light_num = min(g_light_num, MAX_LIGHT_NUM);
	for(int i = 0; i < light_num; ++i){
		PointLight light = g_point_lights[i];
		vec3 light_dir = normalize(light.position - w_pos);		// ���շ���
		vec3 h_dir = normalize(view_dir + light_dir);		// h����

		// ���ռ���
		float D = distribution_ggx(w_normal, h_dir, roughness);
		vec3 F = fresnel_schlick(dot(h_dir, view_dir), f0);
		float G = geometry_smith(w_normal, light_dir, view_dir, roughness);
//		float dfg_denom = 4 * dot(w_normal, view_dir) * dot(w_normal, light_dir);
		float dfg_denom = 4 * max(dot(w_normal, view_dir), 0) * max(dot(w_normal, light_dir), 0) + 0.001;
		vec3 specular = D * F * G / dfg_denom;

		vec3 ks = F;
		vec3 kd = vec3(1) - ks;
		kd *= 1.0 - metalness;	// ��������������ߣ���ֱ�����գ�

		vec3 radiance = light.base_light.color / calc_attenuation(light.atten, length(light.position - w_pos));
		float n_dot_l = dot(w_normal, light_dir);

		color += kd * (albedo / PI + specular) * n_dot_l * radiance;
	}
	vec3 ambiant = vec3(0.03) * albedo * ao;
	color += ambiant;

	g_frag_color = vec4(color, 1);
}

float calc_attenuation(Attenuation atten, float dis){
	return atten.constant + atten.linear * dis + atten.expo * dis * dis;;
}

// ����D��
float distribution_ggx(vec3 n, vec3 h, float roughness){
	float a = roughness * roughness;	// �ݵ�ʿ��ľ��������ʹ�ù��տ�����������Ȼ
	float a2 = a * a;
	float n_o_h = dot(n, h);
	float denom = n_o_h * n_o_h * (a2 - 1) + 1;
	denom = PI * denom * denom;
	return a2 / denom;
}

// ����F��
vec3 fresnel_schlick(float h_o_v, vec3 f0){
	return f0 + (1 - f0) * pow(1 - h_o_v, 5);
}

// ����G��(ֱ�ӹ���)
float geometry_schlick_ggx(float costheta, float roughness){
	float k = (roughness + 1);
	k = k * k/ 8;

	float denom = costheta * (1 - k) + k;

	return costheta / denom;
}

float geometry_smith(vec3 n, vec3 l, vec3 v, float roughness){
	float n_dot_l = max(dot(n, l), 0);
	float n_dot_v = max(dot(n, v), 0);
	return geometry_schlick_ggx(n_dot_l, roughness) * geometry_schlick_ggx(n_dot_v, roughness);
}
