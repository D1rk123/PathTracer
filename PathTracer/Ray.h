#pragma once

#include <glm/glm.hpp>

struct Ray
{
    const glm::vec3 orig;
    const glm::vec3 dir;

    Ray(glm::vec3 orig, glm::vec3 dir) : orig(orig), dir(dir)
    {
    }
};
