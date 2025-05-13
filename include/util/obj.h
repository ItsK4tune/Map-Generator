#ifndef OBJ_H
#define OBJ_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 uv;
    glm::vec3 color;
    glm::vec3 normal;
};

glm::vec3 calculateNormal(glm::vec3 a, glm::vec3 b, glm::vec3 c);
void generateTerrain(std::vector<Vertex> &vertices);

#endif