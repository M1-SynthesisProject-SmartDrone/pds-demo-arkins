#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "gui/events/Events.h"
#include "engine/map/Map.h"
#include "engine/arkins/Arkins.h"

class Engine
{
private:
    const Map m_map;
    Coordinates m_droneCoordinates;
    Arkins m_arkins;

    bool m_isPaused = false;

    // ==== Private methods ====
    void handleEvents(Events& events);
    void resetSimulation();

public:
    Engine(ConfigMap mapConfig);
    ~Engine();

    void update(Events& events);

    // ==== GETTERS ====
    bool isPaused();
    MapInfos getMapInfos();
    Coordinates& getDroneCoordinates();
    vector<Coordinates>& getAttractivePoints();
    vector<Coordinates>& getRepulsivePoints();
    vector<Coordinates>& getTangentialPoints();
    // TODO uniform
};

#endif // __ENGINE_H__