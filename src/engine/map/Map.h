#ifndef __MAP_H__
#define __MAP_H__

#include <vector>

#include "config/ConfigParams.h"
#include "engine/data/Coordinates.h"
#include "MapInfos.h"

/**
 * Create and contains all coordinates, along with the global map infos
 */
struct Map
{
    /**
     * Calling the constructor will load all coordinates from the file provided.
     */
    Map(ConfigMap mapConfig);
    ~Map();

    MapInfos infos;

    Coordinates droneCoordinates;
    std::vector<Coordinates> attractivePoints;
    std::vector<Coordinates> repulsivePoints;
    std::vector<Coordinates> tangentialPoints;
    // TODO uniform fields
};

#endif // __MAP_H__