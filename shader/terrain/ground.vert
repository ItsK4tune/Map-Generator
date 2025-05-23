#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

out vec3 fragPos;
out vec3 fragNormal;
out float height;

uniform vec3 objectPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    height = pos.y;
    fragPos = vec3(model * vec4(objectPosition + pos, 1.0));
    fragNormal = mat3(transpose(inverse(model))) * normal;

    gl_Position = projection * view * vec4(fragPos, 1.0);
}
