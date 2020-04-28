#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "SceneObject.h"
#include "PointLight.h"
#include "Ray.h"
#include "Camera.h"
#include "ImageRgb.h"

class RayTracer
{
    std::unique_ptr<Camera> cam;
    std::vector<std::unique_ptr<SceneObject>> objects;
    std::vector<PointLight> lightSources;

    class workDistributor;

    IntersectionResult testIntersection(const Ray& ray);
    glm::vec3 calcDirectIllumination(const glm::vec3& point, const glm::vec3& intersectionNormal);
    void renderThread(ImageRgb* result, int numSamples, workDistributor* distributor);
    void renderPixel(int x, int y, ImageRgb* result, int numSamples);


public:
    template <class CameraType>
    RayTracer(CameraType&& camera): cam(std::make_unique<CameraType>(std::forward<CameraType>(camera)))
    {
    }

    template <class ObjectType>
    void addObject(ObjectType&& object)
    {
        objects.emplace_back(std::make_unique<ObjectType>(std::forward<ObjectType>(object)));
    }
    void addPointLight(const PointLight& light)
    {
        lightSources.push_back(light);
    }

    ImageRgb renderDepth();
    ImageRgb renderDirectLight(int numSamples);
    ImageRgb render(int numSamples, int numThreads);
};