#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "Plane.h"
#include "PointLight.h"
#include "Ray.h"
#include "Camera.h"
#include "ImageRgb.h"

class RayTracer
{
    std::unique_ptr<Camera> cam;
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;
    std::vector<PointLight> lightSources;

    struct IntersectionResult
    {
        float distance;
        glm::vec3 normal;
        glm::vec3 color;
    };

    IntersectionResult testIntersection(const Ray& ray);
    glm::vec3 calcDirectIllumination(const Ray& ray, const IntersectionResult& intersection);

public:
    RayTracer(std::unique_ptr<Camera> initCam): cam(std::move(initCam))
    {
    }

    void addSphere(const Sphere& sphere)
    {
        spheres.push_back(sphere);
    }
    void addPlane(const Plane& plane)
    {
        planes.push_back(plane);
    }
    void addPointLight(const PointLight& light)
    {
        lightSources.push_back(light);
    }

    ImagePpm renderDepth();
    ImageRgb<float> renderDirectLight(int numSamples);
};