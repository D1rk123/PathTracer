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
#include "ImagePpm.h"

int main()
{
    std::cout << "Hello World!\n";
    RayTracer tracer(std::make_unique<PerspectiveCamera>(700, 500, glm::mat3(1.0f), glm::vec3(0, 0, 0), 70.0f));
    tracer.addSphere(Sphere({ 0,-1,5 }, 0.4f));
    tracer.addSphere(Sphere({ 0,-1,7 }, 0.4f));
    tracer.addSphere(Sphere({ 0,-1,9 }, 0.4f));
    tracer.addSphere(Sphere({ 0,-1,11 }, 0.4f));
    tracer.addPlane(Plane({ 0,1,0 }, -1));

    ImagePpm resultImg = tracer.render();
    resultImg.writeToFile("test.ppm");
}
