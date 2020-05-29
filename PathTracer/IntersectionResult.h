#pragma once

#include <limits>
#include <glm/glm.hpp>

#include "MaterialType.h"

class SceneObject;

struct IntersectionResult
{
    float distance;
    glm::vec3 normal;
    glm::vec3 color;
    MaterialType materialType;
    SceneObject* object;

    IntersectionResult(const float distance, const glm::vec3& normal, const glm::vec3& color, MaterialType materialType, SceneObject* const object)
        : distance(distance), normal(normal), color(color), materialType(materialType), object(object)
    {

    }

    bool foundIntersection()
    {
        return distance != std::numeric_limits<float>::infinity();
    }

    static IntersectionResult makeNoIntersectionFound()
    {
        return IntersectionResult(std::numeric_limits<float>::infinity(), { 0, 0, 0 }, { 0, 0, 0 }, MaterialType::diffuse, nullptr);
    }
};
