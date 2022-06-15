#include "Engine.h"

Engine::Engine(ConfigMap mapConfig, ConfigWindow windowConfig) :
    m_map(mapConfig, windowConfig),
    m_droneCoordinates(m_map.droneCoordinates),
    m_arkins(m_map.attractivePoints, m_map.repulsivePoints, m_map.tangentialPoints, m_map.uniformFields)
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

    // If no attractive point remaining, we finished the simulation
    if (m_arkins.countAttractionPoints() == 0)
    {
        m_isPaused = true;
        return;
    }

    m_arkins.process(m_droneCoordinates);
    Informations& movementInfos = m_arkins.getInfos();
    auto& vect = movementInfos.vector;


    // Invert this vector (goes from destination to drone) and normalize it
    auto inverted = vect.inverted();
    auto direction = inverted.toUnitVector();
    printf("dir x=%f, y=%f, z=%f\n", direction.vx, direction.vy, direction.vz);
    
    // Move the drone
    m_droneCoordinates.x += direction.vx * MOVEMENT_SPEED;
    m_droneCoordinates.y += direction.vy * MOVEMENT_SPEED;
    m_droneCoordinates.z += direction.vz * MOVEMENT_SPEED;
    // m_droneCoordinates.rotation += direction.vr * MOVEMENT_SPEED;

    // handle point removal
    if (movementInfos.inRange)
    {
        m_arkins.deleteAttractivePoint(movementInfos.nearestPointIndex);
    }
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

Coordinates& Engine::getBarycenter()
{
    return m_arkins.getInfos().barycenter;
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

vector<Tangent>& Engine::getTangentialPoints()
{
    return m_arkins.getTangentialPoints();
}

vector<Uniform>& Engine::getUniformFields()
{
    return m_arkins.getUniformPoints();
}
