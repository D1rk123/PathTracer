#pragma once

#include <iostream>
#include <glm/glm.hpp>

struct Ray
{
    glm::vec3 orig;
    glm::vec3 dir;

    Ray(glm::vec3 orig, glm::vec3 dir) : orig(orig), dir(dir)
    {
    }
};

std::ostream& operator<<(std::ostream& os, Ray const& ray);
