#version 330 core
out vec4 FragColor;

in vec3 Normal;

uniform sampler2D texture_diffuse1;

void main()
{    
    FragColor = vec4(abs(Normal), 1.0);
}