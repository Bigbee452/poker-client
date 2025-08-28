#version 330 core
out vec4 FragColor; 

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 Color;

uniform sampler2D guiTexture;

void main(){
    vec4 color = Color/255;
    FragColor = texture(guiTexture, TexCoord)*color;
}