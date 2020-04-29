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

IntersectionResult RayTracer::testIntersection(const Ray& ray)
{
    IntersectionResult result = IntersectionResult::makeNoIntersectionFound();

    for (const auto& object : objects)
    {
        auto foundResult = object->testIntersection(ray);
        if (foundResult.distance < result.distance)
        {
            result = foundResult;
        }
    }
    return result;
}

glm::vec3 RayTracer::calcDirectIllumination(const glm::vec3& point, const glm::vec3& intersectionNormal)
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
            float radiance = light.power * (glm::dot(toLight.dir, intersectionNormal) / (4 * Constants::pi * lightDistance * lightDistance));
            illum += light.color * radiance;
        }
    }
    return illum;
}

ImageRgb RayTracer::renderDepth()
{
    ImageRgb result(cam->resX(), cam->resY());
    for (int x = 0; x < cam->resX(); ++x)
    {
        for (int y = 0; y < cam->resY(); ++y)
        {
            auto intersection = testIntersection(cam->generateRay(x, y));
            result.set(x, y, { intersection.distance, intersection.distance, intersection.distance });
        }
    }
    return result;
}

ImageRgb RayTracer::renderDirectLight(int numSamples)
{
    ImageRgb result(cam->resX(), cam->resY());
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
                    pixelColor += intersection.color * (1 / Constants::pi) * calcDirectIllumination(point, intersection.normal);
                }
            }
            pixelColor /= numSamples;
            result.set(x, y, pixelColor);
        }
    }
    return result;
}

void RayTracer::renderPixel(int x, int y, ImageRgb* result, int numSamples)
{
    std::random_device rd;
    std::mt19937 mersenneTwister(rd());
    std::uniform_real_distribution<float> rouletteSampler(0, 1);
    constexpr float rouletteFactor = 0.85f;
    //glm::vec<3, double> pixelColor(0, 0, 0);
    glm::vec3 pixelColor(0, 0, 0);
    for (int i = 0; i < numSamples; ++i)
    {
        glm::vec3 reflectionScale(1, 1, 1);
        Ray ray = cam->generateRay(x, y);
        int numBounces = 0;
        bool hitMirror = false;
        std::string series;

        while (true)//numBounces < 50)
        {
            numBounces++;
            IntersectionResult intersection = testIntersection(ray);
            if (!intersection.foundIntersection())
            {
                break;
            }
            glm::vec3 intersectionPoint = ray.orig + ray.dir *std::max(0.0f, intersection.distance - Constants::minIntersectionDistance);

            if (intersection.materialType == MaterialType::diffuse)
            {
                reflectionScale *= intersection.color;
                pixelColor += reflectionScale * calcDirectIllumination(intersectionPoint, intersection.normal) * (1 / Constants::pi);

                if (rouletteSampler(mersenneTwister) < rouletteFactor)
                {
                    //make new ray
                    ray.orig = intersectionPoint;
                    ray.dir = glm::ballRand(1.0f);
                    float dotNewNormal = glm::dot(ray.dir, intersection.normal);
                    if (dotNewNormal < 0)
                    {
                        ray.dir = -ray.dir;
                        dotNewNormal = -dotNewNormal;
                    }
                    reflectionScale *= (1 / rouletteFactor) * dotNewNormal * 2;
                }
                else
                {
                    break;
                }
            }
            else if (intersection.materialType == MaterialType::mirror)
            {
                ray.dir = glm::reflect(ray.dir, intersection.normal);
                ray.orig = intersectionPoint + (intersection.normal * 0.001f);
            }
            else if (intersection.materialType == MaterialType::emissive)
            {
                pixelColor += reflectionScale * intersection.color;
                break;
            }
        }
    }
    pixelColor /= numSamples;
    result->set(x, y, pixelColor);
}

void RayTracer::renderThread(ImageRgb* result, int numSamples, workDistributor* distributor)
{
    auto pixel = distributor->getNextPixel();
    while(pixel.first != -1)
    {
        renderPixel(pixel.first, pixel.second, result, numSamples);
        pixel = distributor->getNextPixel();
    }
}

ImageRgb RayTracer::render(int numSamples, int numThreads)
{
    ImageRgb result(cam->resX(), cam->resY());
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