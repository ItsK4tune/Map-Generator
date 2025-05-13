#include "util/obj.h"

#define TERRAIN_SIZE 100
#define HALF_SIZE (TERRAIN_SIZE / 2)

glm::vec3 calculateNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    glm::vec3 edge1 = b - a;
    glm::vec3 edge2 = c - a;
    return glm::normalize(glm::cross(edge1, edge2));
}

void generateTerrain(std::vector<Vertex> &vertices)
{
    std::vector<std::vector<float>> heightMap(TERRAIN_SIZE, std::vector<float>(TERRAIN_SIZE));

    for (int x = 0; x < TERRAIN_SIZE; ++x)
        for (int y = 0; y < TERRAIN_SIZE; ++y)
            heightMap[x][y] = static_cast<float>(rand()) / RAND_MAX;

    glm::vec3 lightPos(0.0f, 5.0f, 0.0f);

    for (int x = 0; x < TERRAIN_SIZE - 1; ++x)
    {
        for (int y = 0; y < TERRAIN_SIZE - 1; ++y)
        {
            float z1 = heightMap[x][y];
            float z2 = heightMap[x][y + 1];
            float z3 = heightMap[x + 1][y];
            float z4 = heightMap[x + 1][y + 1];

            glm::vec3 p1(x - HALF_SIZE, z1, y - HALF_SIZE);
            glm::vec3 p2(x - HALF_SIZE, z2, y + 1 - HALF_SIZE);
            glm::vec3 p3(x + 1 - HALF_SIZE, z3, y - HALF_SIZE);
            glm::vec3 p4(x + 1 - HALF_SIZE, z4, y + 1 - HALF_SIZE);

            glm::vec3 n1 = calculateNormal(p1, p2, p3);
            glm::vec3 n2 = calculateNormal(p3, p2, p4);

            auto computeColor = [&](float height, glm::vec3 normal, glm::vec3 pos) -> glm::vec3
            {
                glm::vec3 lightDir = glm::normalize(lightPos - pos);
                float diffuse = glm::clamp(glm::dot(normal, lightDir), 0.0f, 1.0f);
                glm::vec3 baseColor(height, 0.5f, 1.0f - height);
                return glm::clamp(baseColor * diffuse, 0.0f, 1.0f);
            };

            vertices.push_back({p1, {x / (float)TERRAIN_SIZE, y / (float)TERRAIN_SIZE}, computeColor(z1, n1, p1), n1});
            vertices.push_back({p2, {x / (float)TERRAIN_SIZE, (y + 1) / (float)TERRAIN_SIZE}, computeColor(z2, n1, p2), n1});
            vertices.push_back({p3, {(x + 1) / (float)TERRAIN_SIZE, y / (float)TERRAIN_SIZE}, computeColor(z3, n1, p3), n1});

            vertices.push_back({p3, {(x + 1) / (float)TERRAIN_SIZE, y / (float)TERRAIN_SIZE}, computeColor(z3, n2, p3), n2});
            vertices.push_back({p2, {x / (float)TERRAIN_SIZE, (y + 1) / (float)TERRAIN_SIZE}, computeColor(z2, n2, p2), n2});
            vertices.push_back({p4, {(x + 1) / (float)TERRAIN_SIZE, (y + 1) / (float)TERRAIN_SIZE}, computeColor(z4, n2, p4), n2});
        }
    }
}