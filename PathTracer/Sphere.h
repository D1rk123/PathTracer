#pragma once

#include <limits>
#include <glm/glm.hpp>

#include "Ray.h"
#include "Constants.h"

struct Sphere
{
    const glm::vec3 pos;
    const float radius;
    const glm::vec3 color;

    Sphere(const glm::vec3 pos, const float radius, const glm::vec3 color)
        : pos(pos), radius(radius), color(color)
    {
    }

    float testIntersection(const Ray ray)
    {
        glm::vec3 q = ray.orig - pos;
        //float a = glm::dot(ray.dir, ray.dir);
        //assume ray.dir is a unit vector, so a is 1 and falls out of all equations
        float b = 2.0f * glm::dot(ray.dir, q);
        float c = glm::dot(q, q) - radius * radius;

        float discriminantSquared = b * b - 4.0f * c;
        if (discriminantSquared < 0)
        {
            return std::numeric_limits<float>::infinity();
        }
        const float discriminant = sqrtf(discriminantSquared);
        const float distance1 = (-b + discriminant) / 2.0f;
        const float distance2 = (-b - discriminant) / 2.0f;
        float finalDistance = std::numeric_limits<float>::infinity();
        if (distance1 > Constants::minIntersectionDistance)
        {
            finalDistance = distance1;
        }
        if (distance2 > Constants::minIntersectionDistance && distance2 < finalDistance)
        {
            finalDistance = distance2;
        }
        return finalDistance;
    }
};