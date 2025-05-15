#include "util/object.h"
#include "util/loadShader.h"

#define STB_PERLIN_IMPLEMENTATION
#include "stb/stb_perlin.h"

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

Object::Object(const Mesh &mesh,
               const glm::vec3 &ambient,
               const glm::vec3 &diffuse,
               const glm::vec3 &specular,
               float shininess)
    : m_mesh(mesh), m_material{ambient, diffuse, specular, shininess}
{
}

Object::Object(const Mesh &mesh,
               const Material &material)
    : m_mesh(mesh), m_material(material)
{
}

Object::Object()
    : m_material{{0.1f, 0.1f, 0.1f},
                 {0.5f, 0.5f, 0.5f},
                 {1.0f, 1.0f, 1.0f},
                 32.0f}
{
}

void Object::setMesh(const Mesh &mesh)
{
    m_mesh = mesh;
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

void Object::setShader(const Shader &shader)
{
    m_shader = shader;
}

Mesh &Object::getMesh()
{
    return m_mesh;
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

Shader &Object::getShader()
{
    return m_shader;
}

void Object::use() const
{
    m_shader.use();
}

void Object::draw() const
{
    m_mesh.draw();
}

Mesh::Mesh(const std::vector<Vertex> &verts)
    : vertices(verts)
{
    setupMesh();
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Mesh::draw() const
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Shader::Shader() : ID(0) {}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    ID = createShaderProgram(vertexPath, fragmentPath);
}

Shader::~Shader()
{
    if (ID != 0)
        glDeleteProgram(ID);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &vec) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
}

void Shader::setFloat(const std::string &name, float val) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), val);
}

unsigned int Shader::getID() const
{
    return ID;
}

glm::vec3 calculateNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    glm::vec3 edge1 = b - a;
    glm::vec3 edge2 = c - a;
    return glm::normalize(glm::cross(edge1, edge2));
}

float perlinNoise2D(float x, float y, int octaves = 4, float persistence = 0.5f, float scale = 1.0f)
{
    float total = 0.0f;
    float frequency = scale;
    float amplitude = 1.0f;
    float maxValue = 0.0f; // để chuẩn hóa kết quả về [0,1]

    for (int i = 0; i < octaves; i++)
    {
        float noiseValue = stb_perlin_noise3(x * frequency, y * frequency, 0.0f, 0, 0, 0);
        total += noiseValue * amplitude;

        maxValue += amplitude;
        amplitude *= persistence; // giảm biên độ theo mỗi octave
        frequency *= 2.0f;        // tăng tần số (chi tiết) theo mỗi octave
    }

    return total / maxValue; // chuẩn hóa noise về khoảng [-1,1]
}

void generateTerrain(std::vector<Vertex> &vertices, int terrainSize)
{
    std::vector<std::vector<float>> heightMap(terrainSize, std::vector<float>(terrainSize));

    for (int x = 0; x < terrainSize; ++x)
    {
        for (int y = 0; y < terrainSize; ++y)
        {
            // Thay vì random đơn thuần:
            // heightMap[x][y] = static_cast<float>(rand()) / RAND_MAX * 1.07f;

            float nx = (float)x / (float)terrainSize;
            float ny = (float)y / (float)terrainSize;

            float noiseHeight = perlinNoise2D(nx * 10.0f, ny * 10.0f, 6, 0.5f, 2.0f); // tăng số octave & scale
            noiseHeight = (noiseHeight + 1.0f) / 2.0f;                                // chuẩn hóa về [0,1]
            heightMap[x][y] = noiseHeight * 2.0f;                                     // scale độ cao
        }
    }

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

void createSunVertices(std::vector<Vertex> &vertices)
{
    glm::vec3 frontNormal(0.0f, 0.0f, 1.0f);
    vertices.push_back({{-0.1f, -0.1f, 0.1f}, {0.0f, 0.0f}, frontNormal});
    vertices.push_back({{0.1f, -0.1f, 0.1f}, {1.0f, 0.0f}, frontNormal});
    vertices.push_back({{0.1f, 0.1f, 0.1f}, {1.0f, 1.0f}, frontNormal});
    vertices.push_back({{0.1f, 0.1f, 0.1f}, {1.0f, 1.0f}, frontNormal});
    vertices.push_back({{-0.1f, 0.1f, 0.1f}, {0.0f, 1.0f}, frontNormal});
    vertices.push_back({{-0.1f, -0.1f, 0.1f}, {0.0f, 0.0f}, frontNormal});

    // Mặt sau (-z)
    glm::vec3 backNormal(0.0f, 0.0f, -1.0f);
    vertices.push_back({{-0.1f, -0.1f, -0.1f}, {0.0f, 0.0f}, backNormal});
    vertices.push_back({{-0.1f, 0.1f, -0.1f}, {0.0f, 1.0f}, backNormal});
    vertices.push_back({{0.1f, 0.1f, -0.1f}, {1.0f, 1.0f}, backNormal});
    vertices.push_back({{0.1f, 0.1f, -0.1f}, {1.0f, 1.0f}, backNormal});
    vertices.push_back({{0.1f, -0.1f, -0.1f}, {1.0f, 0.0f}, backNormal});
    vertices.push_back({{-0.1f, -0.1f, -0.1f}, {0.0f, 0.0f}, backNormal});

    // Mặt trái (-x)
    glm::vec3 leftNormal(-1.0f, 0.0f, 0.0f);
    vertices.push_back({{-0.1f, 0.1f, 0.1f}, {1.0f, 1.0f}, leftNormal});
    vertices.push_back({{-0.1f, 0.1f, -0.1f}, {1.0f, 0.0f}, leftNormal});
    vertices.push_back({{-0.1f, -0.1f, -0.1f}, {0.0f, 0.0f}, leftNormal});
    vertices.push_back({{-0.1f, -0.1f, -0.1f}, {0.0f, 0.0f}, leftNormal});
    vertices.push_back({{-0.1f, -0.1f, 0.1f}, {0.0f, 1.0f}, leftNormal});
    vertices.push_back({{-0.1f, 0.1f, 0.1f}, {1.0f, 1.0f}, leftNormal});

    // Mặt phải (+x)
    glm::vec3 rightNormal(1.0f, 0.0f, 0.0f);
    vertices.push_back({{0.1f, 0.1f, 0.1f}, {1.0f, 1.0f}, rightNormal});
    vertices.push_back({{0.1f, -0.1f, -0.1f}, {0.0f, 0.0f}, rightNormal});
    vertices.push_back({{0.1f, 0.1f, -0.1f}, {1.0f, 0.0f}, rightNormal});
    vertices.push_back({{0.1f, -0.1f, -0.1f}, {0.0f, 0.0f}, rightNormal});
    vertices.push_back({{0.1f, 0.1f, 0.1f}, {1.0f, 1.0f}, rightNormal});
    vertices.push_back({{0.1f, -0.1f, 0.1f}, {0.0f, 1.0f}, rightNormal});

    // Mặt dưới (-y)
    glm::vec3 bottomNormal(0.0f, -1.0f, 0.0f);
    vertices.push_back({{-0.1f, -0.1f, -0.1f}, {0.0f, 0.0f}, bottomNormal});
    vertices.push_back({{0.1f, -0.1f, -0.1f}, {1.0f, 0.0f}, bottomNormal});
    vertices.push_back({{0.1f, -0.1f, 0.1f}, {1.0f, 1.0f}, bottomNormal});
    vertices.push_back({{0.1f, -0.1f, 0.1f}, {1.0f, 1.0f}, bottomNormal});
    vertices.push_back({{-0.1f, -0.1f, 0.1f}, {0.0f, 1.0f}, bottomNormal});
    vertices.push_back({{-0.1f, -0.1f, -0.1f}, {0.0f, 0.0f}, bottomNormal});

    // Mặt trên (+y)
    glm::vec3 topNormal(0.0f, 1.0f, 0.0f);
    vertices.push_back({{-0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, topNormal});
    vertices.push_back({{-0.1f, 0.1f, 0.1f}, {0.0f, 1.0f}, topNormal});
    vertices.push_back({{0.1f, 0.1f, 0.1f}, {1.0f, 1.0f}, topNormal});
    vertices.push_back({{0.1f, 0.1f, 0.1f}, {1.0f, 1.0f}, topNormal});
    vertices.push_back({{0.1f, 0.1f, -0.1f}, {1.0f, 0.0f}, topNormal});
    vertices.push_back({{-0.1f, 0.1f, -0.1f}, {0.0f, 0.0f}, topNormal});
}