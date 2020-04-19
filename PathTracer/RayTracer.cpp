#include <limits>
#include <algorithm>
#include <thread>
#include <mutex>

#include <glm/gtc/random.hpp>

#include "Constants.h"

#include "RayTracer.h"

class RayTracer::workDistributor{
    const int resolutionX, resolutionY, lastPixel;
    int currPixelNum;
    int percentFinished;
    std::mutex pixelNumLock;

public:
    workDistributor(const int resolutionX, const int resolutionY) :
        resolutionX(resolutionX), resolutionY(resolutionY), lastPixel(resolutionX * resolutionY),
        currPixelNum(-1), percentFinished(0)
    {}

    std::pair<int, int> getNextPixel()
    {
        std::lock_guard guard(pixelNumLock);
        currPixelNum++;
        if (currPixelNum >= lastPixel)
        {
            return { -1, -1 };
        }
        
        int currPercentFinished = static_cast<int>(std::round(static_cast<double>(currPixelNum) * 100 / lastPixel));
        if (currPercentFinished != percentFinished)
        {
            percentFinished = currPercentFinished;
            std::cout << percentFinished << "%" << std::endl;
        }
        return { currPixelNum % resolutionX, currPixelNum / resolutionX };
    }
};

RayTracer::IntersectionResult RayTracer::testIntersection(const Ray& ray)
{
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

glm::vec3 RayTracer::calcDirectIllumination(const glm::vec3& point, const IntersectionResult& intersection)
{
    glm::vec3 illum(0, 0, 0);

    for (const auto& light : lightSources)
    {
        auto relLightPos = light.pos - point;
        float lightDistance = glm::length(relLightPos);

        Ray toLight(point, relLightPos/lightDistance);
        auto lightIntersection = testIntersection(toLight);
        if (lightIntersection.distance > lightDistance)
        {
            float radiance = light.power * (glm::dot(toLight.dir, intersection.normal)) / (4 * Constants::pi * lightDistance * lightDistance);
            illum += light.color * radiance;
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
                    glm::vec3 point = ray.orig + ray.dir * intersection.distance;
                    pixelColor += intersection.color * calcDirectIllumination(point, intersection);
                }
            }
            pixelColor /= numSamples;
            result.set(x, y, { pixelColor.x, pixelColor.y, pixelColor.z });
        }
    }
    return result;
}

void RayTracer::renderPixel(int x, int y, ImageRgb<float>* result, int numSamples)
{
    std::random_device rd;
    std::mt19937 mersenneTwister(rd());
    std::uniform_real_distribution<float> rouletteSampler(0, 1);
    const float rouletteFactor = 0.85f;
    glm::vec3 pixelColor(0, 0, 0);
    for (int i = 0; i < numSamples; ++i)
    {
        IntersectionResult intersection;
        glm::vec3 reflectionScale(1, 1, 1);
        bool continueTracing = true;
        Ray ray = cam->generateRay(x, y);

        while (continueTracing)
        {
            intersection = testIntersection(ray);
            if (intersection.distance == std::numeric_limits<float>::infinity())
            {
                break;
            }
            glm::vec3 intersectionPoint = ray.orig + ray.dir * intersection.distance;

            reflectionScale *= intersection.color;
            pixelColor += reflectionScale * calcDirectIllumination(intersectionPoint, intersection);

            if (rouletteSampler(mersenneTwister) > rouletteFactor)
            {
                //make new ray
                float normalDot = glm::dot(ray.dir, intersection.normal);
                ray.orig = intersectionPoint;
                ray.dir = glm::ballRand(1.0f);
                reflectionScale *= std::fabsf(normalDot) * (1 / Constants::pi) * (1 / rouletteFactor);
                if (normalDot < 0)
                {
                    ray.dir.x = -ray.dir.x;
                }
            }
            else
            {
                continueTracing = false;
            }
        }
    }
    pixelColor /= numSamples;
    result->set(x, y, { pixelColor.x, pixelColor.y, pixelColor.z });
}

void RayTracer::renderThread(ImageRgb<float>* result, int numSamples, workDistributor* distributor)
{
    auto pixel = distributor->getNextPixel();
    while(pixel.first != -1)
    {
        renderPixel(pixel.first, pixel.second, result, numSamples);
        pixel = distributor->getNextPixel();
    }
}

ImageRgb<float> RayTracer::render(int numSamples, int numThreads)
{
    ImageRgb<float> result(cam->resX(), cam->resY());
    workDistributor distributor(cam->resX(), cam->resY());
    std::vector<std::thread> threads(numThreads);
    for (auto iter = threads.begin(); iter != threads.end(); ++iter)
    {
        *iter = std::thread([this, resultPtr = &result, numSamples, distributorPtr = &distributor]()
            {
                this->renderThread(resultPtr, numSamples, distributorPtr);
            });
    }
    for (auto iter = threads.begin(); iter != threads.end(); ++iter)
    {
        iter->join();
    }

    return result;
}