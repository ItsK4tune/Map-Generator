#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum class LightType
{
    Point,
    Directional
};

class Light
{
public:
    Light();
    Light(glm::vec3 pos,
          glm::vec3 color,
          glm::vec3 ambient,
          glm::vec3 diffuse,
          glm::vec3 specular,
          LightType type = LightType::Point);

    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    glm::vec3 getAmbient() const;
    glm::vec3 getDiffuse() const;
    glm::vec3 getSpecular() const;
    LightType getType() const;

    void setPosition(const glm::vec3 &pos);
    void setColor(const glm::vec3 &color);
    void setAmbient(const glm::vec3 &ambient);
    void setDiffuse(const glm::vec3 &diffuse);
    void setSpecular(const glm::vec3 &specular);
    void setType(LightType type);

private:
    glm::vec3 m_pos;
    glm::vec3 m_color;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;

    LightType m_type = LightType::Point;
};

#endif