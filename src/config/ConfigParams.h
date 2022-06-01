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

/**
 * The struct containing all config params
 */
struct ConfigParams
{
    ConfigWindow window;

    ConfigParams(ConfigWindow window) : window(window)
    {}
};

#endif // __CONFIGPARAMS_H__