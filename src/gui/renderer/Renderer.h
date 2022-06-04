#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "config/ConfigParams.h"
#include "engine/data/Coordinates.h"

class Renderer
{
private:
    static inline const std::string DRONE_IMAGE_NAME = "drone.png";

    const ConfigWindow m_windowConfig;
    const ConfigImage m_imageConfig;
    const std::filesystem::path m_imageFolderPath;

    // This is kept as we load an image for this element
    sf::Texture m_droneTexture;
    sf::RectangleShape m_droneSprite;

    /**
     * Load a texture from the base filename wanted (do not put the folder name)
     */
    sf::Texture loadTexture(std::string baseFilename);

public:
    /**
     * This will check if the config is valid (correct image folder path, etc.)
     */ 
    Renderer(ConfigImage imageConfig, ConfigWindow windowConfig);
    ~Renderer();

    void renderDrone(Coordinates& droneCoordinates, sf::RenderWindow& window);
    void renderAttractivePoints(std::vector<Coordinates>& attractivePoints, sf::RenderWindow& window);
    void renderRepulsivePoints(std::vector<Coordinates>& repulsivePoints, sf::RenderWindow& window);
    void renderTangentialPoints(std::vector<Coordinates>& tangentialPoints, sf::RenderWindow& window);
    // TODO : uniform field
};

#endif // __RENDERER_H__