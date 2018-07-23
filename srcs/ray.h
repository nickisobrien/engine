#pragma once

#include "engine.h"

class Ray
{
    public:
        Ray(const glm::vec3 &position, const glm::vec3 &direction);
        void step(float scale);
        const glm::vec3& getEnd() const;
        float getLength() const;
    private:
        glm::vec3 rayStart;
        glm::vec3 rayEnd;
        glm::vec3 direction;
};