#include "ImageRgb.h"

#define TINYEXR_IMPLEMENTATION
#include <tinyexr.h>

std::vector<unsigned char> ImageRgb::convertTo8Bit() const
{
    std::vector<unsigned char> result(pixelData.size());

    auto sortedPixelData = pixelData;
    std::sort(sortedPixelData.begin(), sortedPixelData.end());
    double max = (sortedPixelData[static_cast<int>(sortedPixelData.size() * 0.5)] * 2
        + sortedPixelData[static_cast<int>(sortedPixelData.size() * 0.95)]) / 2;
    std::cout << max << std::endl;

    //double max = static_cast<double>(*std::max_element(pixelData.cbegin(), pixelData.cend()));

    //double max = 0.01;

    for (unsigned int i = 0; i < pixelData.size(); i++)
    {
        const double normalized = static_cast<double>(pixelData[i]) / max;
        const double scaled = std::clamp<double>(std::round(normalized * 255), 0, 255);
        result[i] = static_cast<unsigned char>(scaled);
    }

    return result;
}

void ImageRgb::writeToPpm(const std::string path)
{
    std::ofstream fileStream(path.c_str());

    fileStream << "P3" << std::endl;
    fileStream << resolutionX << std::endl;
    fileStream << resolutionY << std::endl;
    fileStream << "255" << std::endl;

    std::vector<unsigned char> image8bit = convertTo8Bit();
    for (auto i = image8bit.cbegin(); i != image8bit.cend(); ++i)
    {
        fileStream << static_cast<unsigned int>(*i) << " ";
    }
}

std::vector<float> ImageRgb::getChannel(ColorChannel channel) const
{
    int channelOffset = 0;
    switch (channel)
    {
    case ColorChannel::red:
        channelOffset = 0;
        break;
    case ColorChannel::green:
        channelOffset = 1;
        break;
    case ColorChannel::blue:
        channelOffset = 2;
        break;
    }

    const auto resultSize = resolutionX * resolutionY;
    std::vector<float> result(resolutionX * resolutionY);
    for (PixPosT i = 0; i < resultSize; ++i)
    {
        result[i] = pixelData[i * 3 + channelOffset];
    }
    return result;
}

void ImageRgb::writeToExr(const std::string path)
{
    EXRHeader header;
    InitEXRHeader(&header);

    EXRImage exrImage;
    InitEXRImage(&exrImage);

    std::vector<float> redChannel = getChannel(ColorChannel::red);
    std::vector<float> greenChannel = getChannel(ColorChannel::green);
    std::vector<float> blueChannel = getChannel(ColorChannel::blue);

    float* image_ptr[3];
    image_ptr[0] = &(blueChannel[0]); // B
    image_ptr[1] = &(greenChannel[0]); // G
    image_ptr[2] = &(redChannel[0]); // R

    exrImage.num_channels = 3;
    exrImage.images = (unsigned char**)image_ptr;
    exrImage.width = static_cast<int>(resolutionX);
    exrImage.height = static_cast<int>(resolutionY);

    header.num_channels = 3;

    std::vector<EXRChannelInfo> channelsInfo(3);
    header.channels = &(channelsInfo[0]);
    // Must be (A)BGR order, since most of EXR viewers expect this channel order.
    strncpy_s(header.channels[0].name, "B", 255); header.channels[0].name[strlen("B")] = '\0';
    strncpy_s(header.channels[1].name, "G", 255); header.channels[1].name[strlen("G")] = '\0';
    strncpy_s(header.channels[2].name, "R", 255); header.channels[2].name[strlen("R")] = '\0';

    std::vector<int> pixelTypes(3, TINYEXR_PIXELTYPE_FLOAT);
    std::vector<int> requestedPixelTypes(3, TINYEXR_PIXELTYPE_HALF);

    header.pixel_types = &(pixelTypes[0]);
    header.requested_pixel_types = &(requestedPixelTypes[0]);

    const char* err = nullptr;
    int ret = SaveEXRImageToFile(&exrImage, &header, path.c_str(), &err);
    if (ret != TINYEXR_SUCCESS) {
        fprintf(stderr, "Save EXR err: %s\n", err);
        FreeEXRErrorMessage(err); // free's buffer for an error message
    }
}