#include "Renderer.h"

#include <loguru/loguru.hpp>

using namespace std;
namespace fs = std::filesystem;

Renderer::Renderer(ConfigImage imageConfig) :
    m_imageConfig(imageConfig),
    m_imageFolderPath(imageConfig.folderPath)
{
    // TODO check if folder exists
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
}

void Renderer::renderDrone(Coordinates& droneCoordinates, sf::RenderWindow& window)
{

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
