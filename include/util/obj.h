#ifndef OBJ_H
#define OBJ_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 uv;
    // glm::vec3 color;
    glm::vec3 normal;
};

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

class Object
{
public:
    Object(const glm::vec3 &ambient,
           const glm::vec3 &diffuse,
           const glm::vec3 &specular,
           float shininess);

    Object(const Material &material);

    Object(const std::vector<Vertex> &vertices,
           const glm::vec3 &ambient,
           const glm::vec3 &diffuse,
           const glm::vec3 &specular,
           float shininess);

    Object(const std::vector<Vertex> &vertices,
           const Material &material);

    Object();

    void setVertices(const std::vector<Vertex> &vertices);
    void setMaterial(const Material &material);
    void setAmbient(const glm::vec3 &ambient);
    void setDiffuse(const glm::vec3 &diffuse);
    void setSpecular(const glm::vec3 &specular);
    void setShininess(float shininess);

    std::vector<Vertex> &getVertex();
    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
    float getShininess() const;

private:
    std::vector<Vertex> m_vertices;
    Material m_material;
};

glm::vec3 calculateNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
void generateTerrain(std::vector<Vertex> &vertices, int terrainSize);

#endif