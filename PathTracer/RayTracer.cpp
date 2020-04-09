#include <limits>
#include <algorithm>

#include "RayTracer.h"

float RayTracer::testIntersection(Ray ray)
{
    //std::cout << ray.orig.x << ", " << ray.orig.y << ", " << ray.orig.z << std::endl;
    //return (ray.orig.x > 0) != (ray.orig.y > 0);
    float distance = std::numeric_limits<float>::infinity();
    for (Sphere sphere : spheres)
    {
        distance = std::fmin(distance, sphere.testIntersection(ray));
    }
    for (Plane plane : planes)
    {
        distance = std::fmin(distance, plane.testIntersection(ray));
    }
    return distance;
}

ImagePpm RayTracer::render()
{
    ImagePpm result(cam->resX(), cam->resY());
    for (int x = 0; x < cam->resX(); ++x)
    {
        for (int y = 0; y < cam->resY(); ++y)
        {
            //std::cout << x << ", " << y << std::endl;
            const float intersectionDistance = testIntersection(cam->generateRay(x, y));
            const float intersectionNormalized = std::clamp<float>((1.0f - intersectionDistance/10.0f)*255, 0, 255);
            const unsigned char shade = static_cast<unsigned char>(intersectionNormalized);
            //const unsigned char shade = (intersectionDistance == std::numeric_limits<float>::infinity()) ? 0 : 255;
            result.set(x, y, { shade, shade, shade });
        }
    }
    return result;
}