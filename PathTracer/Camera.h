#pragma once

#include <glm/glm.hpp>
#include "Ray.h"

class Camera
{
protected:
    const int resolutionX, resolutionY;
    const glm::mat3 orientation;
    const glm::vec3 position;

    Camera(int resolutionX, int resolutionY, glm::mat3 orientation, glm::vec3 position)
        : resolutionX(resolutionX), resolutionY(resolutionY), orientation(orientation), position(position)
    {
    }

public:
    int resX() const
    {
        return resolutionX;
    }
    int resY() const
    {
        return resolutionY;
    }

    virtual Ray generateRay(int pixelX, int pixelY) = 0;
};
