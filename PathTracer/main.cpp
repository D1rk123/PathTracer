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
    RayTracer tracer(std::make_unique<PerspectiveCamera>(2100, 1500, glm::mat3(1.0f), glm::vec3(0, 0, 0), 70.0f));
    for (int i = 0; i < 15; ++i)
    {
        tracer.addSphere(Sphere({ -0.75 + 0.55*i, -0.6 + 0.4*i, 4 + 2*i }, 0.4f, { 1, 1, 1 }));
    }
    tracer.addPlane(Plane({ 0,1,0 }, -1, { 1, 1, 1 }));
    tracer.addPointLight(PointLight({  0, 2.5,  2 }, { 1, 0.3, 0.1 }, 10));
    tracer.addPointLight(PointLight({  0, 2.0,  8.5 }, { 0, 0.2, 1.0 }, 3));
    tracer.addPointLight(PointLight({ -1, 0.1, 0 }, { 0, 1, 0 }, 10));
    tracer.addPointLight(PointLight({  1, 0.1, 0 }, { 0, 0, 1 }, 10));

    //auto depthImg = tracer.renderDepth();
    //ImagePpm::writeToPpm(depthImg, "depth.ppm");
    auto directIllumImg = tracer.renderDirectLight(20);
    ImagePpm::writeToPpm(directIllumImg.convertTo8Bit(), "directIllum.ppm");
}