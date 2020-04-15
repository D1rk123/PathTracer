#pragma once

#include <iostream>

#include <limits>
#include <glm/glm.hpp>

#include "Constants.h"

#include "Ray.h"

struct Plane
{
    const glm::vec3 normal;
    const float offset;
    const glm::vec3 color;

    Plane(const glm::vec3 normal, const float offset, const glm::vec3 color)
        : normal(normal), offset(offset), color(color)
    {
    }

    //Two-sided intersection test
    float testIntersection(const Ray ray)
    {
        const float origDistance = glm::dot(ray.orig, normal);
        const float localDir = glm::dot(ray.dir, normal);
        if (localDir == 0)
        {
            return std::numeric_limits<float>::infinity();
        }
        const float distance = (offset - origDistance) / localDir;

        if (distance < Constants::minIntersectionDistance)
        {
            return std::numeric_limits<float>::infinity();
        }
        return distance;
    }
};