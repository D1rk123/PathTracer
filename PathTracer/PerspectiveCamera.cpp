#include "PerspectiveCamera.h"

Ray PerspectiveCamera::generateRay(int pixelX, int pixelY)
{
    glm::vec3 rayDir(
        (((static_cast<float>(pixelX) + 0.5f) / resolutionX) * 2.0f - 1.0f)* maxXExtent,
        (((static_cast<float>(pixelY) + 0.5f) / resolutionY) * 2.0f - 1.0f)* maxYExtent,
        1.0f
    );
    return Ray(position, orientation * glm::normalize(rayDir));
}