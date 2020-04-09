#pragma once

#include <glm/glm.hpp>
#include "Camera.h"

class OrthographicCamera : public Camera
{

public:
    OrthographicCamera(int resolutionX, int resolutionY, glm::mat3 orientation, glm::vec3 position)
        : Camera(resolutionX, resolutionY, orientation, position)
    {
    }

    Ray generateRay(int pixelX, int pixelY) override;
};