#version 330 core
out vec4 FragColor; 

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;
    sampler2D texture_diffuse3;
    sampler2D texture_specular1;
    sampler2D texture_specular2;
}; 

uniform Material material;
uniform vec3 lightColor;
uniform vec3 light_position;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
  
void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_position - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec3 textureColor = texture(material.texture_diffuse1, TexCoord).xyz;
    vec3 ambient  = vec3(0.3)*textureColor;
    vec3 diffuse  = textureColor*(diff * material.diffuse);
    vec3 specular = (spec * material.specular);

    vec3 result = (ambient+diffuse+specular)*material.diffuse*lightColor;
    FragColor = vec4(result, 1.0)*1.7;
}