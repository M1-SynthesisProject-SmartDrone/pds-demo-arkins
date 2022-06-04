#include "Renderer.h"

#include <loguru/loguru.hpp>

using namespace std;
namespace fs = std::filesystem;

Renderer::Renderer(ConfigImage imageConfig, ConfigWindow windowConfig, MapInfos mapInfos) :
    m_imageConfig(imageConfig),
    m_windowConfig(windowConfig),
    m_imageFolderPath(imageConfig.folderPath),
    m_mapInfos(mapInfos)
{
    if (!fs::exists(m_imageFolderPath) || !fs::is_directory(m_imageFolderPath))
    {
        stringstream ss;
        ss << "The folder \"" << imageConfig.folderPath << "\" does not exists";
        throw runtime_error(ss.str());
    }
    // m_spriteDrone
    m_droneTexture = loadTexture(DRONE_IMAGE_NAME);
    m_droneSprite.setTexture(&m_droneTexture);
    // Size of the drone depends on the window size
    float droneWidth = m_windowConfig.width / 10.0;
    float droneHeight = m_windowConfig.height / 10.0;
    m_droneSprite.setSize(sf::Vector2f(droneWidth, droneHeight));
}

Renderer::~Renderer()
{}

sf::Texture Renderer::loadTexture(string baseFilename)
{
    fs::path baseFilePath(baseFilename);
    auto fullPath = m_imageFolderPath / baseFilePath;
    sf::Texture texture;
    if(!texture.loadFromFile(fullPath.string()))
    {
        stringstream ss;
        ss << "Error while loading texture on path " << fullPath;
        throw runtime_error(ss.str());
    }
    texture.setSmooth(m_imageConfig.enableSmooth);
    return texture;
}

void Renderer::renderDrone(Coordinates& droneCoordinates, sf::RenderWindow& window)
{
    m_droneSprite.setPosition(sf::Vector2f(calculateXPos(droneCoordinates.x), calculateYPos(droneCoordinates.y)));
    m_droneSprite.setRotation(droneCoordinates.rotation);
    window.draw(m_droneSprite);
}

void Renderer::renderAttractivePoints(std::vector<Coordinates>& attractivePoints, sf::RenderWindow& window)
{

}

void Renderer::renderRepulsivePoints(std::vector<Coordinates>& repulsivePoints, sf::RenderWindow& window)
{

}

void Renderer::renderTangentialPoints(std::vector<Coordinates>& tangentialPoints, sf::RenderWindow& window)
{

}

float Renderer::calculateXPos(float x)
{
    auto& m = m_mapInfos;
    return (x / (m.maxX - m.minX)) * m_windowConfig.width;
}

float Renderer::calculateYPos(float y)
{
    auto& m = m_mapInfos;
    return (y / (m.maxY - m.minY)) * m_windowConfig.height;
}