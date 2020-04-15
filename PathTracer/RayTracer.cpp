#include <limits>
#include <algorithm>

#include "Constants.h"

#include "RayTracer.h"

RayTracer::IntersectionResult RayTracer::testIntersection(const Ray& ray)
{
    //std::cout << ray.orig.x << ", " << ray.orig.y << ", " << ray.orig.z << std::endl;
    //return (ray.orig.x > 0) != (ray.orig.y > 0);
    IntersectionResult result;
    result.distance = std::numeric_limits<float>::infinity();
    result.color = glm::vec3(1.0, 0.0, 0.0);

    for (Plane plane : planes)
    {
        float foundDistance = plane.testIntersection(ray);
        if (foundDistance < result.distance)
        {
            result.distance = foundDistance;
            result.color = plane.color;
            result.normal = plane.normal;
        }
    }
    for (Sphere sphere : spheres)
    {
        float foundDistance = sphere.testIntersection(ray);
        if (foundDistance < result.distance)
        {
            result.distance = foundDistance;
            result.color = sphere.color;
            auto intersectionPoint = ray.orig + ray.dir * foundDistance;
            result.normal = glm::normalize(intersectionPoint - sphere.pos);
        }
    }
    return result;
}

glm::vec3 RayTracer::calcDirectIllumination(const Ray& ray, const IntersectionResult& intersection)
{
    glm::vec3 illum(0, 0, 0);
    constexpr float scale = 2 * 4 * Constants::pi * Constants::pi;
    glm::vec3 point = ray.orig + ray.dir * intersection.distance;

    for (const auto& light : lightSources)
    {
        auto relLightPos = light.pos - point;
        float lightDistance = glm::length(relLightPos);

        Ray toLight(point, relLightPos/lightDistance);
        auto lightIntersection = testIntersection(toLight);
        if (lightIntersection.distance > lightDistance)
        {
            float radiance = light.power * (glm::dot(toLight.dir, intersection.normal)) / (scale * lightDistance * lightDistance);
            illum += (light.color * intersection.color) * radiance;
        }
    }
    return illum;
}

ImagePpm RayTracer::renderDepth()
{
    ImagePpm result(cam->resX(), cam->resY());
    for (int x = 0; x < cam->resX(); ++x)
    {
        for (int y = 0; y < cam->resY(); ++y)
        {
            //std::cout << x << ", " << y << std::endl;
            auto intersection = testIntersection(cam->generateRay(x, y));
            const float intersectionNormalized = std::clamp<float>((1.0f - intersection.distance/10.0f)*255, 0, 255);
            const unsigned char shade = static_cast<unsigned char>(intersectionNormalized);
            //const unsigned char shade = (intersectionDistance == std::numeric_limits<float>::infinity()) ? 0 : 255;
            result.set(x, y, { shade, shade, shade });
        }
    }
    return result;
}

ImageRgb<float> RayTracer::renderDirectLight(int numSamples)
{
    ImageRgb<float> result(cam->resX(), cam->resY());
    for (int x = 0; x < cam->resX(); ++x)
    {
        for (int y = 0; y < cam->resY(); ++y)
        {
            glm::vec3 pixelColor(0, 0, 0);
            for (int i = 0; i < numSamples; ++i)
            {
                const auto ray = cam->generateRay(x, y);
                auto intersection = testIntersection(ray);
                if (intersection.distance != std::numeric_limits<float>::infinity())
                {
                    pixelColor += calcDirectIllumination(ray, intersection);
                }
            }
            pixelColor /= numSamples;
            result.set(x, y, { pixelColor.x, pixelColor.y, pixelColor.z });
        }
    }
    return result;
}