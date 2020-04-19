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
    class workDistributor;

    IntersectionResult testIntersection(const Ray& ray);
    glm::vec3 calcDirectIllumination(const glm::vec3& point, const IntersectionResult& intersection);
    void renderThread(ImageRgb<float>* result, int numSamples, workDistributor* distributor);
    void renderPixel(int x, int y, ImageRgb<float>* result, int numSamples);


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
    ImageRgb<float> render(int numSamples, int numThreads);
};