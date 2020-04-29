// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

RayTracer makeCornellBoxSceneOneColorBigShadow()
{
    RayTracer tracer(PerspectiveCamera(1050, 750, glm::mat3(1.0f), glm::vec3(0, 0, 0), 90.0f));
    tracer.addObject(Plane({ 0,1,0 }, -1, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Plane({ 0,-1,0 }, -1, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Plane({ 1,0,0 }, -1, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Plane({ -1,0,0 }, -1, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Plane({ 0,0,-1 }, -2, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Sphere({ 0, 0.4, 1 }, 0.2f, { 0.85, 0.85, 0.85 }));
    tracer.addPointLight(PointLight({ 0, 0.95, 1 }, { 0.85, 0.85, 0.85 }, 2));

    return tracer;
}

RayTracer makeCornellBoxScene()
{
    RayTracer tracer(PerspectiveCamera(1050, 750, glm::mat3(1.0f), glm::vec3(0, 0, 0), 90.0f));
    tracer.addObject(Plane({ 0,1,0 }, -1, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Plane({ 0,-1,0 }, -1, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Plane({ 1,0,0 }, -1, { 0.85, 0, 0 }));
    tracer.addObject(Plane({ -1,0,0 }, -1, { 0, 0.85, 0 }));
    tracer.addObject(Plane({ 0,0,-1 }, -2, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Sphere({ -0.6, -0.7, 1.2}, 0.3f, { 0.85, 0.85, 0.85 }));
    tracer.addObject(Sphere({ 0.4, -0.7, 1.4 }, 0.3f, { 0.85, 0.85, 0.85 }, MaterialType::mirror));
    tracer.addObject(Sphere({ 0.1, 0.1, 0.9 }, 0.2f, { 0.85, 0.85, 0.85 }, MaterialType::mirror));
    //tracer.addObject(Sphere({ 0.55, -0.35, 1.25}, 0.25f, { 0.85, 0.85, 0.85 }));
    //tracer.addObject(Sphere({ 0.0, -0.8, 1.0 }, 0.15f, { 0.85, 0.85, 0.85 }, MaterialType::mirror));
    tracer.addPointLight(PointLight({ 0, 0.95, 1 }, { 0.85, 0.85, 0.85 }, 2));

    return tracer;
}


int main()
{
    auto tracer = makeCornellBoxScene();

    //auto depthImg = tracer.renderDepth();
    //ImagePpm::writeToPpm(depthImg, "depth.ppm");
    auto directIllumImg = tracer.renderDirectLight(1);
    directIllumImg.writeToPpm("directIllum.ppm");
    auto resultImg = tracer.render(250, 11);
    resultImg.writeToExr("fullResult.exr");
    resultImg.writeToPpm("fullResult.ppm");
}
