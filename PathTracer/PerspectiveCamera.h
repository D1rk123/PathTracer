#pragma once

#pragma once

#include <glm/glm.hpp>
#include "Camera.h"

class PerspectiveCamera : public Camera
{
    const float horizontalFOV;
    const float maxXExtent;
    const float maxYExtent;

public:
    PerspectiveCamera(int resolutionX, int resolutionY, glm::mat3 orientation, glm::vec3 position, float horizontalFOV)
        : Camera(resolutionX, resolutionY, orientation, position),
        horizontalFOV(horizontalFOV),
        maxXExtent(tanf(horizontalFOV / 2.0f)),
        maxYExtent((maxXExtent*resolutionY) / resolutionX)
    {
    }

    Ray generateRay(int pixelX, int pixelY) override;
};
