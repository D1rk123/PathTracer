#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "Plane.h"
#include "Ray.h"
#include "Camera.h"
#include "ImagePpm.h"

class RayTracer
{
    std::unique_ptr<Camera> cam;
    std::vector<Sphere> spheres;
    std::vector<Plane> planes;

    float testIntersection(Ray ray);

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

    ImagePpm render();
};