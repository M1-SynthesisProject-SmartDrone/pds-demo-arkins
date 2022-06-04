#include "Renderer.h"

#include <loguru/loguru.hpp>

using namespace std;
namespace fs = std::filesystem;

Renderer::Renderer(ConfigImage imageConfig, ConfigWindow windowConfig) :
    m_imageConfig(imageConfig),
    m_windowConfig(windowConfig),
    m_imageFolderPath(imageConfig.folderPath)
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
    m_droneSprite.setSize(sf::Vector2f(50.0f, 50.0f));
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
    m_droneSprite.setPosition(sf::Vector2f(droneCoordinates.x, droneCoordinates.y));
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
