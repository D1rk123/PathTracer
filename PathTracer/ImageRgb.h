#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

#include <glm/glm.hpp>

class ImageRgb
{
    std::vector<float> pixelData;
    using PixPosT = decltype(pixelData)::size_type;
    const PixPosT resolutionX, resolutionY;

    PixPosT getPixelPos(const PixPosT x, const PixPosT y) const
    {
        return (x + ((resolutionY - 1) - y) * resolutionX) * 3;
    }

public:

    ImageRgb(const PixPosT resolutionX, const PixPosT resolutionY)
        : resolutionX(resolutionX), resolutionY(resolutionY), pixelData(resolutionX * resolutionY * 3, 0)
    {
    }

    void set(const PixPosT x, const PixPosT y, glm::vec3 color)
    {
        const auto pixelPos = getPixelPos(x, y);
        pixelData[pixelPos] = color.r;
        pixelData[pixelPos + 1] = color.g;
        pixelData[pixelPos + 2] = color.b;
    }

    glm::vec3 get(const PixPosT x, const PixPosT y) const
    {
        const auto pixelPos = getPixelPos(x, y);

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