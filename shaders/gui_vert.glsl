#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec4 aColor;

out vec3 FragPos;
out vec2 TexCoord;
out vec4 Color;

uniform mat4 translate;

void main()
{
    gl_Position = translate * vec4(aPos, 1.0);
    FragPos = (translate * vec4(aPos, 1.0)).xyz;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;  
    Color = aColor;
} 