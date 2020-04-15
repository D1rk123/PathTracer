// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <limits>
#include <glm/glm.hpp>
#include "RayTracer.h"
#include "Sphere.h"
#include "Ray.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "ImageRgb.h"

int main()
{
    std::cout << "Hello World!\n";
    RayTracer tracer(std::make_unique<PerspectiveCamera>(700, 500, glm::mat3(1.0f), glm::vec3(0, 0, 0), 70.0f));
    tracer.addSphere(Sphere({ 0,0.3,5 }, 0.4f, { 1, 1, 1 }));
    tracer.addSphere(Sphere({ 0,0.3,7 }, 0.4f, { 1, 1, 1 }));
    tracer.addSphere(Sphere({ 0,0.3,9 }, 0.4f, { 1, 1, 1 }));
    tracer.addSphere(Sphere({ 0,0.3,11 }, 0.4f, { 1, 1, 1 }));
    tracer.addPlane(Plane({ 0,1,0 }, -1, { 1, 1, 1 }));
    tracer.addPointLight(PointLight({ 0,3,2 }, { 1, 0, 0 }, 10));
    tracer.addPointLight(PointLight({ -1,0.1,0 }, { 0, 1, 0 }, 10));
    tracer.addPointLight(PointLight({ 1,0.1,0 }, { 0, 0, 1 }, 10));

    //auto depthImg = tracer.renderDepth();
    //ImagePpm::writeToPpm(depthImg, "depth.ppm");
    auto directIllumImg = tracer.renderDirectLight();
    ImagePpm::writeToPpm(directIllumImg.convertTo8Bit(), "directIllum.ppm");
}
