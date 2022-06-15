#ifndef __MAP_H__
#define __MAP_H__

#include <vector>

#include "config/ConfigParams.h"
#include "engine/data/Coordinates.h"
#include "engine/data/Tangent.h"
#include "engine/data/Uniform.h"
#include "MapInfos.h"

/**
 * Create and contains all coordinates, along with the global map infos
 */
struct Map
{
    /**
     * Calling the constructor will load all coordinates from the file provided.
     */
    Map(ConfigMap mapConfig, ConfigWindow windowConfig);
    ~Map();

    MapInfos infos;

    Coordinates droneCoordinates;
    std::vector<Coordinates> attractivePoints;
    std::vector<Coordinates> repulsivePoints;
    std::vector<Tangent> tangentialPoints;
    std::vector<Uniform> uniformFields;
};

#endif // __MAP_H__