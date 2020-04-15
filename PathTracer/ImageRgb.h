#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>

template <typename ChannelType>
class ImageRgb
{
    const int resolutionX, resolutionY;
    std::vector<ChannelType> pixelData;

    int getPixelPos(const int x, const int y) const
    {
        return (x + ((resolutionY - 1) - y) * resolutionX) * 3;
    }

public:
    struct Color
    {
        ChannelType r;
        ChannelType g;
        ChannelType b;

        Color(ChannelType r, ChannelType g, ChannelType b) : r(r), g(g), b(b)
        {
        }
    };

    ImageRgb(const int resolutionX, const int resolutionY)
        : resolutionX(resolutionX), resolutionY(resolutionY), pixelData(resolutionX* resolutionY *3, 0)
    {
    }

    void set(const int x, const int y, Color color)
    {
        const int pixelPos = getPixelPos(x, y);
        pixelData[pixelPos] = color.r;
        pixelData[pixelPos + 1] = color.g;
        pixelData[pixelPos + 2] = color.b;
    }
    void set(const int i, ChannelType channelValue)
    {
        pixelData[i] = channelValue;
    }

    Color get(const int x, const int y) const
    {
        const int pixelPos = getPixelPos(x, y);

        return Color(pixelData[pixelPos] , pixelData[pixelPos + 1], pixelData[pixelPos + 2]);
    }
    ChannelType get(const int i) const
    {
        return pixelData[i];
    }

    ImageRgb<unsigned char> convertTo8Bit()
    {
        ImageRgb<unsigned char> result(resolutionX, resolutionY);

        /*auto sortedPixelData = pixelData;
        std::sort(sortedPixelData.begin(), sortedPixelData.end());
        double max = sortedPixelData[sortedPixelData.size() * 0.8];
        std::cout << max << std::endl;*/
        
        //double max = static_cast<double>(*std::max_element(pixelData.cbegin(), pixelData.cend()));

        double max = 0.01;

        for (unsigned int i=0; i<pixelData.size(); i++)
        {
            const double normalized = static_cast<double>(pixelData[i]) / max;
            const double scaled = std::clamp<double>(std::round(normalized*255), 0, 255);
            result.set(i, static_cast<unsigned char>(scaled));
        }

        return result;
    }

    static void writeToPpm(const ImageRgb<unsigned char>& image, const std::string path)
    {
        std::ofstream fileStream(path.c_str());

        fileStream << "P3" << std::endl;
        fileStream << image.resolutionX << std::endl;
        fileStream << image.resolutionY << std::endl;
        fileStream << "255" << std::endl;
        for (auto i = image.pixelData.cbegin(); i != image.pixelData.cend(); ++i)
        {
            fileStream << static_cast<unsigned int>(*i) << " ";
        }
    }
};

using ImagePpm = ImageRgb<unsigned char>;