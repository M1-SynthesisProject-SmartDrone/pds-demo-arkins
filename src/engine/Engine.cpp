#include "Engine.h"

Engine::Engine(ConfigMap mapConfig) :
    m_map(mapConfig),
    m_droneCoordinates(m_map.droneCoordinates),
    m_arkins(m_map.attractivePoints, m_map.repulsivePoints, m_map.tangentialPoints)
{}

Engine::~Engine()
{}

void Engine::update(Events& events)
{
    handleEvents(events);

    if (m_isPaused)
    {
        return;
    }

    m_droneCoordinates.x += 1.0;
}

void Engine::handleEvents(Events& events)
{
    if (events.pausePressed)
    {
        m_isPaused = !m_isPaused;
    }
    if (events.resetPressed)
    {
        resetSimulation();
    }
}

void Engine::resetSimulation()
{
    m_isPaused = true;
    m_droneCoordinates = m_map.droneCoordinates;
    m_arkins.resetAttractivePoints(m_map.attractivePoints);
}

// ==== GETTERS =====
bool Engine::isPaused()
{
    return m_isPaused;   
}

MapInfos Engine::getMapInfos()
{
    return m_map.infos;
}

Coordinates& Engine::getDroneCoordinates()
{
    return m_droneCoordinates;
}

vector<Coordinates>& Engine::getAttractivePoints()
{
    return m_arkins.getAttractionPoints();
}

vector<Coordinates>& Engine::getRepulsivePoints()
{
    return m_arkins.getRepulsionPoints();
}

vector<Coordinates>& Engine::getTangentialPoints()
{
    return m_arkins.getTangentialPoints();
}
