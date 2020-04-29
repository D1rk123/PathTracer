#pragma once

#include <iostream>

#include <limits>
#include <glm/glm.hpp>

#include "Constants.h"
#include "Ray.h"
#include "SceneObject.h"

class Plane : public SceneObject
{
    const glm::vec3 normal;
    const float offset;
    const glm::vec3 color;
public:

    Plane(const glm::vec3 normal, const float offset, const glm::vec3 color, MaterialType materialType = MaterialType::diffuse)
        : SceneObject(materialType), normal(normal), offset(offset), color(color)
    {
    }

    //Two-sided intersection test
    IntersectionResult testIntersection(const Ray& ray) override
    {
        const float origDistance = glm::dot(ray.orig, normal);
        const float localDir = glm::dot(ray.dir, normal);
        if (localDir == 0)
        {
            return IntersectionResult::makeNoIntersectionFound();
        }
        const float distance = (offset - origDistance) / localDir;

        if (distance < Constants::minIntersectionDistance)
        {
            return IntersectionResult::makeNoIntersectionFound();
        }
        return IntersectionResult(distance, normal, color, materialType);
    }
};