#include "util/obj.h"

Object::Object(const glm::vec3 &ambient,
               const glm::vec3 &diffuse,
               const glm::vec3 &specular,
               float shininess)
    : m_material{ambient, diffuse, specular, shininess}
{
}

Object::Object(const Material &material)
    : m_material(material)
{
}

Object::Object(const std::vector<Vertex> &vertices,
               const glm::vec3 &ambient,
               const glm::vec3 &diffuse,
               const glm::vec3 &specular,
               float shininess)
    : m_vertices(vertices), m_material{ambient, diffuse, specular, shininess}
{
}

Object::Object(const std::vector<Vertex> &vertices,
               const Material &material)
    : m_vertices(vertices), m_material(material)
{
}

Object::Object()
    : m_material{{0.1f, 0.1f, 0.1f},
                 {0.5f, 0.5f, 0.5f},
                 {1.0f, 1.0f, 1.0f},
                 32.0f}
{
}

void Object::setVertices(const std::vector<Vertex> &vertices)
{
    m_vertices = vertices;
}

void Object::setMaterial(const Material &material)
{
    m_material = material;
}

void Object::setAmbient(const glm::vec3 &ambient)
{
    m_material.ambient = ambient;
}

void Object::setDiffuse(const glm::vec3 &diffuse)
{
    m_material.diffuse = diffuse;
}

void Object::setSpecular(const glm::vec3 &specular)
{
    m_material.specular = specular;
}

void Object::setShininess(float shininess)
{
    m_material.shininess = shininess;
}

std::vector<Vertex> &Object::getVertex()
{
    return m_vertices;
}

glm::vec3 Object::getAmbient() const
{
    return m_material.ambient;
}

glm::vec3 Object::getDiffuse() const
{
    return m_material.diffuse;
}

glm::vec3 Object::getSpecular() const
{
    return m_material.specular;
}

float Object::getShininess() const
{
    return m_material.shininess;
}

glm::vec3 calculateNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    glm::vec3 edge1 = b - a;
    glm::vec3 edge2 = c - a;
    return glm::normalize(glm::cross(edge1, edge2));
}

void generateTerrain(std::vector<Vertex> &vertices, int terrainSize)
{
    std::vector<std::vector<float>> heightMap(terrainSize, std::vector<float>(terrainSize));

    for (int x = 0; x < terrainSize; ++x)
        for (int y = 0; y < terrainSize; ++y)
            heightMap[x][y] = static_cast<float>(rand()) / RAND_MAX * 1.07f;

    for (int x = 0; x < terrainSize - 1; ++x)
    {
        for (int y = 0; y < terrainSize - 1; ++y)
        {
            float z1 = heightMap[x][y];
            float z2 = heightMap[x][y + 1];
            float z3 = heightMap[x + 1][y];
            float z4 = heightMap[x + 1][y + 1];

            glm::vec3 p1(x - terrainSize / 2, z1, y - terrainSize / 2);
            glm::vec3 p2(x - terrainSize / 2, z2, y + 1 - terrainSize / 2);
            glm::vec3 p3(x + 1 - terrainSize / 2, z3, y - terrainSize / 2);
            glm::vec3 p4(x + 1 - terrainSize / 2, z4, y + 1 - terrainSize / 2);

            glm::vec3 n1 = calculateNormal(p1, p2, p3);
            glm::vec3 n2 = calculateNormal(p3, p2, p4);

            vertices.push_back({p1, {x / (float)terrainSize, y / (float)terrainSize}, n1});
            vertices.push_back({p2, {x / (float)terrainSize, (y + 1) / (float)terrainSize}, n1});
            vertices.push_back({p3, {(x + 1) / (float)terrainSize, y / (float)terrainSize}, n1});

            vertices.push_back({p3, {(x + 1) / (float)terrainSize, y / (float)terrainSize}, n2});
            vertices.push_back({p2, {x / (float)terrainSize, (y + 1) / (float)terrainSize}, n2});
            vertices.push_back({p4, {(x + 1) / (float)terrainSize, (y + 1) / (float)terrainSize}, n2});
        }
    }
}