#include "util/light.h"

Light::Light()
    : Object(),
      m_pos(0.0f),
      m_color(1.0f),
      m_type(LightType::Point)
{
}

Light::Light(glm::vec3 pos,
             glm::vec3 color,
             LightType type)
    : Object(),
      m_pos(pos),
      m_color(color),
      m_type(type)
{
}

void Light::setPosition(const glm::vec3 &pos) { m_pos = pos; }
void Light::setColor(const glm::vec3 &color) { m_color = color; }
void Light::setType(LightType type) { m_type = type; }

glm::vec3 Light::getPosition() const { return m_pos; }
glm::vec3 Light::getColor() const { return m_color; }
LightType Light::getType() const { return m_type; }
