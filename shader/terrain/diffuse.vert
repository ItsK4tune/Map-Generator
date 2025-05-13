#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 FragPos;   // Vị trí fragment trong world space
out vec3 Normal;    // Normal trong world space
out float ModelHeight; // Chiều cao trong model space (từ aPos.y)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    Normal = normalize(mat3(transpose(inverse(model))) * aNormal);

    ModelHeight = aPos.y;

    TexCoord = aUV;
    gl_Position = projection * view * worldPos;
}