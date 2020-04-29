#pragma once

#include <limits>
#include <glm/glm.hpp>

#include "SceneObject.h"
#include "Ray.h"
#include "Constants.h"

class Sphere : public SceneObject
{
    const glm::vec3 pos;
    const float radius;
    const glm::vec3 color;

public:
    Sphere(const glm::vec3 pos, const float radius, const glm::vec3 color, MaterialType materialType = MaterialType::diffuse)
        : SceneObject(materialType), pos(pos), radius(radius), color(color)
    {
    }

    IntersectionResult testIntersection(const Ray& ray) override
    {
        glm::vec3 q = ray.orig - pos;
        //float a = glm::dot(ray.dir, ray.dir);
        //assume ray.dir is a unit vector, so a is 1 and falls out of all equations
        float b = 2.0f * glm::dot(ray.dir, q);
        if (b > 0)
        {
            return IntersectionResult::makeNoIntersectionFound();
        }
        float c = glm::dot(q, q) - radius * radius;

        float discriminantSquared = b * b - 4.0f * c;
        if (discriminantSquared < 0)
        {
            return IntersectionResult::makeNoIntersectionFound();
        }
        const float discriminant = sqrtf(discriminantSquared);
        const float distance1 = (-b + discriminant) / 2.0f;
        const float distance2 = (-b - discriminant) / 2.0f;
        float finalDistance = std::numeric_limits<float>::infinity();
        if (distance1 > 0)
        {
            finalDistance = distance1;
        }
        if (distance2 > 0 && distance2 < finalDistance)
        {
            finalDistance = distance2;
        }
        if (finalDistance == std::numeric_limits<float>::infinity())
        {
            return IntersectionResult::makeNoIntersectionFound();
        }

        auto intersectionPoint = ray.orig + ray.dir * finalDistance;
        auto normal = glm::normalize(intersectionPoint - pos);

        return IntersectionResult(finalDistance, normal, color, materialType);
    }
};