#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <glm/glm.hpp>

class ImageRgb
{
    const int resolutionX, resolutionY;
    std::vector<float> pixelData;

    int getPixelPos(const int x, const int y) const
    {
        return (x + ((resolutionY - 1) - y) * resolutionX) * 3;
    }

public:

    ImageRgb(const int resolutionX, const int resolutionY)
        : resolutionX(resolutionX), resolutionY(resolutionY), pixelData(resolutionX* resolutionY *3, 0)
    {
    }

    void set(const int x, const int y, glm::vec3 color)
    {
        const int pixelPos = getPixelPos(x, y);
        pixelData[pixelPos] = color.r;
        pixelData[pixelPos + 1] = color.g;
        pixelData[pixelPos + 2] = color.b;
    }

    glm::vec3 get(const int x, const int y) const
    {
        const int pixelPos = getPixelPos(x, y);

        return glm::vec3(pixelData[pixelPos], pixelData[pixelPos + 1], pixelData[pixelPos + 2]);
    }

    std::vector<unsigned char> convertTo8Bit() const;

    void writeToPpm(const std::string path);

    enum class ColorChannel
    {
        red,
        green,
        blue
    };

    std::vector<float> getChannel(ColorChannel channel) const;

        void writeToExr(const std::string path);
};