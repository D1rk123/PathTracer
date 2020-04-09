#pragma once

#include <iostream>

#include <limits>
#include <glm/glm.hpp>
#include "Ray.h"

struct Plane
{
    const glm::vec3 normal;
    const float offset;

    Plane(glm::vec3 normal, float offset) : normal(normal), offset(offset)
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

        if (distance < 0)
        {
            return std::numeric_limits<float>::infinity();
        }
        return distance;
    }
};