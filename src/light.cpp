#include "util/light.h"

Light::Light()
    : m_pos(0.0f), m_color(1.0f),
      m_ambient(0.1f), m_diffuse(0.5f), m_specular(1.0f),
      m_type(LightType::Point)
{
}

Light::Light(glm::vec3 pos,
             glm::vec3 color,
             glm::vec3 ambient,
             glm::vec3 diffuse,
             glm::vec3 specular,
             LightType type)
    : m_pos(pos), m_color(color),
      m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
      m_type(type)
{
}

glm::vec3 Light::getPosition() const { return m_pos; }
glm::vec3 Light::getColor() const { return m_color; }
glm::vec3 Light::getAmbient() const { return m_ambient; }
glm::vec3 Light::getDiffuse() const { return m_diffuse; }
glm::vec3 Light::getSpecular() const { return m_specular; }
LightType Light::getType() const { return m_type; }

void Light::setPosition(const glm::vec3 &pos) { m_pos = pos; }
void Light::setColor(const glm::vec3 &color) { m_color = color; }
void Light::setAmbient(const glm::vec3 &ambient) { m_ambient = ambient; }
void Light::setDiffuse(const glm::vec3 &diffuse) { m_diffuse = diffuse; }
void Light::setSpecular(const glm::vec3 &specular) { m_specular = specular; }
void Light::setType(LightType type) { m_type = type; }