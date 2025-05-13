#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
in float ModelHeight;

out vec4 FragColor;

uniform vec3 lightPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diffuse = clamp(dot(norm, lightDir), 0.0, 1.0) * 2.0;

    vec3 baseColor = vec3(0.0, ModelHeight, 1.0 - ModelHeight);

    vec3 finalColor = clamp(baseColor * diffuse, 0.0, 1.0);

    FragColor = vec4(finalColor, 1.0);
}