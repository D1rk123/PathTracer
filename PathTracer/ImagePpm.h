#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class ImagePpm
{
    const int width, height;
    std::vector<unsigned char> pixelData;

    int getPixelPos(const int x, const int y) const
    {
        return (x + ((height - 1) - y) * width) * 3;
    }

public:
    struct Color
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;

        Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b)
        {
        }
    };

    static constexpr unsigned char initialValue = 255;

    ImagePpm(const int width, const int height)
        : width(width), height(height), pixelData(width*height*3, initialValue)
    {
    }

    void writeToFile(const std::string path) const
    {
        std::ofstream fileStream(path.c_str());

        fileStream << "P3" << std::endl;
        fileStream << width << std::endl;
        fileStream << height << std::endl;
        fileStream << "255" << std::endl;
        for (auto i = pixelData.cbegin(); i != pixelData.cend(); ++i)
        {
            fileStream << static_cast<unsigned int>(*i) << " ";
        }
    }

    void set(const int x, const int y, Color color)
    {
        const int pixelPos = getPixelPos(x, y);
        pixelData[pixelPos] = color.r;
        pixelData[pixelPos + 1] = color.g;
        pixelData[pixelPos + 2] = color.b;
    }

    Color get(const int x, const int y) const
    {
        const int pixelPos = getPixelPos(x, y);

        return Color(pixelData[pixelPos] , pixelData[pixelPos + 1], pixelData[pixelPos + 2]);
    }
};