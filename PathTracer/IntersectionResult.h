#pragma once

#include <limits>
#include <glm/glm.hpp>

#include "MaterialType.h"

struct IntersectionResult
{
    float distance;
    glm::vec3 normal;
    glm::vec3 color;
    MaterialType materialType;

    IntersectionResult(const float distance, const glm::vec3& normal, const glm::vec3& color, MaterialType materialType)
        : distance(distance), normal(normal), color(color), materialType(materialType)
    {

    }

    bool foundIntersection()
    {
        return distance != std::numeric_limits<float>::infinity();
    }

    static IntersectionResult makeNoIntersectionFound()
    {
        return IntersectionResult(std::numeric_limits<float>::infinity(), { 0, 0, 0 }, { 0, 0, 0 }, MaterialType::diffuse);
    }
};
