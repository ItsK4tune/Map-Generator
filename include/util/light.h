#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "object.h" // Đảm bảo include file khai báo class Object

enum class LightType
{
    Point,
    Directional
};

class Light : public Object
{
public:
    Light();
    Light(glm::vec3 pos,
          glm::vec3 color,
          LightType type = LightType::Point);

    void setPosition(const glm::vec3 &pos);
    void setColor(const glm::vec3 &color);
    void setType(LightType type);

    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    LightType getType() const;

private:
    glm::vec3 m_pos;
    glm::vec3 m_color;
    LightType m_type;
};

#endif