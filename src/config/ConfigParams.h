#ifndef __CONFIGPARAMS_H__
#define __CONFIGPARAMS_H__

#include <string>

struct ConfigWindow
{
    std::string name;
    int width;
    int height;
    int framerate;
};

struct ConfigImage
{
    std::string folderPath;
};

/**
 * The struct containing all config params
 */
struct ConfigParams
{
    ConfigImage image;

    ConfigWindow window;

    ConfigParams(ConfigWindow window, ConfigImage image) : window(window), image(image)
    {}
};

#endif // __CONFIGPARAMS_H__