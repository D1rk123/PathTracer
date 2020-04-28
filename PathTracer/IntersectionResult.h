#pragma once

#include <limits>
#include <glm/glm.hpp>

struct IntersectionResult
{
    float distance;
    glm::vec3 normal;
    glm::vec3 color;

    IntersectionResult()
    {
    }

    IntersectionResult(const float distance, const glm::vec3& normal, const glm::vec3& color)
        : distance(distance), normal(normal), color(color)
    {

    }

    bool foundIntersection()
    {
        return distance == std::numeric_limits<float>::infinity();
    }

    static IntersectionResult makeNoIntersectionFound()
    {
        return IntersectionResult(std::numeric_limits<float>::infinity(), { 0, 0, 0 }, { 0, 0, 0 });
    }
};
