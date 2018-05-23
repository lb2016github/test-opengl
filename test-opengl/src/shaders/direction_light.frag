#version 330                                                                        
                                                                                    
in vec2 o_tex_coor0;                                                                  
in vec3 o_normal;                                                                    
in vec3 o_w_pos;                                                                  
                                                                                    
out vec4 out_color;                                                                 
                                                                                    
struct DirectionalLight                                                             
{                                                                                   
    vec3 color;                                                                     
    float ambiant_intensity;                                                         
    float diffuse_intensity;                                                         
    vec3 direction;                                                                 
};                                                                                  
                                                                                    
uniform DirectionalLight dir_light;                                         
uniform sampler2D g_sampler;                                                         
uniform vec3 eye_pos;                                                          
uniform float materia_specular_intensity;                                                
uniform float specular_power;                                                       
                                                                                    
void main()                                                                         
{                                                                                   
    vec4 Ambientcolor = vec4(dir_light.color * dir_light.ambiant_intensity, 1.0f);
    vec3 Lightdirection = -dir_light.direction;                             
    vec3 Normal = normalize(o_normal);                                               
                                                                                    
    float DiffuseFactor = dot(Normal, Lightdirection);                              
                                                                                    
    vec4 Diffusecolor  = vec4(0, 0, 0, 0);                                          
    vec4 Specularcolor = vec4(0, 0, 0, 0);                                          
                                                                                    
    if (DiffuseFactor > 0) {                                                        
        Diffusecolor = vec4(dir_light.color * dir_light.diffuse_intensity * DiffuseFactor, 1.0f);
                                                                                    
        vec3 VertexToEye = normalize(eye_pos - o_w_pos);                     
        vec3 LightReflect = normalize(reflect(dir_light.direction, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);                      
        if (SpecularFactor > 0) {                                                   
            SpecularFactor = pow(SpecularFactor, specular_power);
            Specularcolor = vec4(dir_light.color * materia_specular_intensity * SpecularFactor, 1.0f);
        }                                                                           
    }                                                                               
                                                                                    
    out_color = texture2D(g_sampler, o_tex_coor0.xy) *                                 
                (Ambientcolor + Diffusecolor + Specularcolor);                      
}
