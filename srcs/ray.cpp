#include "ray.h"

Ray::Ray(const glm::vec3& position, const glm::vec3& direction)
{
    this->rayStart = glm::vec3(position);
    this->rayEnd = glm::vec3(position);
    this->direction = glm::vec3(direction);
}

void Ray::step(float scale)
{
    float yaw = glm::radians(this->direction.y + 90);
    float pitch = glm::radians(this->direction.x);

    auto& p = this->rayEnd;

    p.x -= glm::cos(yaw) * scale;
    p.z -= glm::sin(yaw) * scale;
    p.y -= glm::tan(pitch) * scale;
}

const glm::vec3& Ray::getEnd() const
{
    return this->rayEnd;
}

float Ray::getLength() const
{
    return glm::distance(this->rayStart, this->rayEnd);
}