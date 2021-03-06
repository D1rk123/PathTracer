#include <iostream>
#include <limits>
#include <glm/glm.hpp>
#include "RayTracer.h"
#include "Sphere.h"
#include "plane.h"
#include "Ray.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "ImageRgb.h"

RayTracer makeFloatingBallsScene()
{
    RayTracer tracer(PerspectiveCamera(2100, 1500, glm::mat3(1.0f), glm::vec3(0, 0, 0), 70.0f));
    for (int i = 0; i < 15; ++i)
    {
        tracer.addObject(Sphere({ -0.75 + 0.55 * i, -0.6 + 0.4 * i, 4 + 2 * i }, 0.4f, { 1, 1, 1 }));
    }
    tracer.addObject(Plane({ 0,1,0 }, -1, { 1, 1, 1 }));
    tracer.addPointLight(PointLight({ 0, 2.5,  2 }, { 1, 0.3, 0.1 }, 10));
    tracer.addPointLight(PointLight({ 0, 2.0,  8.5 }, { 0, 0.2, 1.0 }, 3));
    tracer.addPointLight(PointLight({ -1, 0.1, 0 }, { 0, 1, 0 }, 10));
    tracer.addPointLight(PointLight({ 1, 0.1, 0 }, { 0, 0, 1 }, 10));
    
    return tracer;
}

RayTracer makeCornellBoxScene()
{
    RayTracer tracer(PerspectiveCamera(700, 500, glm::mat3(1.0f), glm::vec3(0, 0, 0), 90.0f));
    tracer.addObject(Plane({ 0,1,0 }, -1, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Plane({ 0,-1,0 }, -1, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Plane({ 1,0,0 }, -1, { 0.85, 0, 0 }));
    tracer.addObject(Plane({ -1,0,0 }, -1, { 0, 0.85, 0 }));
    tracer.addObject(Plane({ 0,0,-1 }, -2, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Plane({ 0,0,1 }, -0.01f, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Sphere({ -0.6, -0.7, 1.1}, 0.3f, { 0.85, 0.85, 0.85 }, MaterialType::diffuse));
    tracer.addObject(Sphere({ 0.68, -0.55, 1.1 }, 0.2f, { 0.85, 0.85, 0.85 }, MaterialType::glass));
    tracer.addObject(Sphere({ 0.0, -0.8, 1.0 }, 0.2f, { 0.85, 0.85, 0.85 }, MaterialType::mirror));
    tracer.addObject(Sphere({ 0, 1.0, 1 }, 0.3f, { 20, 20, 20 }, MaterialType::emissive));

    return tracer;
}


int main()
{
    auto tracer = makeCornellBoxScene();

    auto depthImg = tracer.renderDepth();
    depthImg.writeToExr("depth.exr");

    // The arguments are number of rays per pixel and number of threads
    auto resultImg = tracer.render(250, 11);
    resultImg.writeToExr("fullResult.exr");
}
