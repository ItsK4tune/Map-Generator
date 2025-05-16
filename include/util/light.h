#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include "util/object.h"

enum class LightType
{
    Point,
    Directional
};

class Light : public Object
{
public:
    Light();
    Light(
        glm::vec3 color,
        LightType type = LightType::Point);

    void setColor(const glm::vec3 &color);
    void setType(LightType type);

    glm::vec3 getColor() const;
    LightType getType() const;

private:
    glm::vec3 m_color;
    LightType m_type;
};

#endif