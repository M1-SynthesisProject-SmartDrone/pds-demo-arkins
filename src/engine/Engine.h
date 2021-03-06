#ifndef __ENGINE_H__
#define __ENGINE_H__

#include "gui/events/Events.h"
#include "engine/map/Map.h"
#include "engine/arkins/Arkins.h"

class Engine
{
private:
    static constexpr float MOVEMENT_SPEED = 1.0f;

    const Map m_map;
    Coordinates m_droneCoordinates;
    Arkins m_arkins;

    bool m_isPaused = false;

    // ==== Private methods ====
    void handleEvents(Events& events);
    void resetSimulation();

public:
    Engine(ConfigMap mapConfig, ConfigWindow windowConfig);
    ~Engine();

    void update(Events& events);

    // ==== GETTERS ====
    bool isPaused();
    MapInfos getMapInfos();
    Coordinates& getBarycenter();
    Coordinates& getDroneCoordinates();
    vector<Coordinates>& getAttractivePoints();
    vector<Coordinates>& getRepulsivePoints();
    vector<Coordinates>& getTangentialPoints();
    vector<Coordinates>& getUniformFields();
};

#endif // __ENGINE_H__