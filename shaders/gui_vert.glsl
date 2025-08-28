#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor;

out vec2 TexCoord;
out vec4 Color;

uniform mat4 translate;
uniform mat4 projection;

void main()
{
    vec4 position = projection*translate*vec4(aPos, 1.0);
    position.y = -position.y;
    gl_Position = position;
    TexCoord = aTexCoord;  
    Color = aColor;
} 