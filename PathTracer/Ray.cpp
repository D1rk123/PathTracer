#include "Ray.h"

#include <glm/gtx/string_cast.hpp>

std::ostream& operator<<(std::ostream& os, Ray const& ray) {
    return os << "ray.orig(" << glm::to_string(ray.orig) << "), ray.dir(" << glm::to_string(ray.dir) << ")";
}