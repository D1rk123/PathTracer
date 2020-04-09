#include "OrthographicCamera.h"

Ray OrthographicCamera::generateRay(int pixelX, int pixelY)
{
    glm::vec3 rayPos(
        ((static_cast<float>(pixelX) + 0.5f) / resolutionX) * 2.0f - 1.0f,
        ((static_cast<float>(pixelY) + 0.5f) / resolutionY) * 2.0f - 1.0f,
        0.0f
    );
    return Ray((orientation * rayPos) + position, orientation * glm::vec3(0.0f, 0.0f, 1.0f));
}