#version 460 core

in vec3 fragPos;
in vec3 fragNormal;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform vec3 ambient;
uniform vec3 diffuse;
uniform vec3 specular;
uniform float shininess;

void main()
{
    vec3 lowColor = vec3(0.1, 0.4, 0.1);
    vec3 highColor = vec3(0.9, 0.9, 0.9);
    float t = clamp(fragPos.y / 1.07, 0.0, 1.0);

    vec3 objectColor = mix(lowColor, highColor, t);

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuseLight = diffuse * diff * objectColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specularLight = specular * spec; // Không nên nhân với objectColor ở đây

    vec3 ambientLight = ambient * objectColor;

    vec3 result = (ambientLight + diffuseLight + specularLight) * lightColor;
    color = vec4(result, 1.0);
}

void a()
{
    vec3 normal = normalize(fragNormal);
    color = vec4(normal * 0.5 + 0.5, 1.0); // từ [-1,1] sang [0,1]
}
