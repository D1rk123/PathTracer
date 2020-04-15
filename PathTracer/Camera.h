#pragma once

#include <random>
#include <glm/glm.hpp>
#include "Ray.h"

class Camera
{
protected:
    const int resolutionX, resolutionY;
    const glm::mat3 orientation;
    const glm::vec3 position;
    std::mt19937 mersenneTwister;
    std::uniform_real_distribution<float> offsetSampler;

    Camera(int resolutionX, int resolutionY, glm::mat3 orientation, glm::vec3 position)
        : resolutionX(resolutionX), resolutionY(resolutionY), orientation(orientation), position(position),
        offsetSampler(0, 1), mersenneTwister(std::random_device()())
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
