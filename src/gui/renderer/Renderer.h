#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include <filesystem>

#include <SFML/Graphics.hpp>

#include "config/ConfigParams.h"
#include "engine/data/Coordinates.h"
#include "engine/data/Tangent.h"
#include "engine/data/Uniform.h"
#include "engine/map/MapInfos.h"

class Renderer
{
private:
    // ==== IMAGE FILENAMES ====
    static inline const std::string DRONE_IMAGE_NAME = "drone.png";
    static inline const std::string PAUSE_IMAGE_NAME = "pause.png";

    // ==== COLORS ====
    static inline const sf::Color GRID_COLOR = sf::Color(105, 105, 105);;
    static inline const sf::Color ATTRACTIVE_POINT_COLOR = sf::Color::Blue;
    static inline const sf::Color BARYCENTER_POINT_COLOR = sf::Color::Green;
    static inline const sf::Color REPULSIVE_POINT_COLOR = sf::Color::Red;
    static inline const sf::Color TANGENTIAL_POINT_COLOR = sf::Color(255, 127, 80); // Orange
    static inline const sf::Color UNIFORM_FIELD_COLOR = sf::Color(212, 19, 209); // Purple

    // ==== SIZES ====
    static inline const float GRID_THICKNESS = 2.0f;
    static inline const float CIRCLE_THICKNESS = 3.0f;
    static inline const float UNIFORM_THICKNESS = 3.0f;

    const ConfigWindow m_windowConfig;
    const ConfigImage m_imageConfig;
    const MapInfos m_mapInfos;
    const std::filesystem::path m_imageFolderPath;

    // This is kept as we load an image for this element
    static inline constexpr float DRONE_SIZE_RATIO = 0.07f;
    sf::Texture m_droneTexture;
    sf::RectangleShape m_droneSprite;

    // Pause icon
    static inline constexpr float PAUSE_SIZE_RATIO = 0.05f;
    sf::Texture m_pauseTexture;
    sf::RectangleShape m_pauseSprite;

    // We don't want to calculate some values over and over
    float m_pointRadius;

    // ==== PRIVATE METHODS ====
    /**
     * Load a texture from the base filename wanted (do not put the folder name)
     */
    sf::Texture loadTexture(std::string baseFilename);

    // ---- Position & size calculation from the map & window infos ----
    float calculateXPos(float x);
    float calculateYPos(float y);
    sf::Vector2f calculatePos(const Coordinates& coordinates);
    float calculateRadius(float r);

    // ---- Shape creation ----
    /**
     * Create a circle with the origin at the center.
     * If "colorOutline" is set to false, it will create a disk (with color filling all).
     * Otherwise, a circle with only the outline will be created 
     * (with the fill color set to Transparent).
     */
    sf::CircleShape createCircle(float radius, const sf::Color& color, bool colorOutline = false);

    /**
     * Create a rectangle with the origin at the center.
     */
    sf::RectangleShape createRectangle(float width, float height, const sf::Color& color);

public:
    /**
     * This will check if the config is valid (correct image folder path, etc.)
     */ 
    Renderer(ConfigImage imageConfig, ConfigWindow windowConfig, MapInfos mapInfos);
    ~Renderer();

    /**
     * Draw a grid in on the window (for debugging puposes)
     */
    void renderGrid(sf::RenderWindow& window);
    /**
     * Draw little elements, such as pause icon
     */
    void renderMisc(sf::RenderWindow& window, bool isPaused);
    void renderbarycenter(Coordinates& barycenter, sf::RenderWindow& window);
    void renderDrone(Coordinates& droneCoordinates, sf::RenderWindow& window);
    void renderAttractivePoints(std::vector<Coordinates>& attractivePoints, sf::RenderWindow& window, float radius);
    void renderRepulsivePoints(std::vector<Coordinates>& repulsivePoints, sf::RenderWindow& window, float effectRadius);
    void renderTangentialPoints(std::vector<Tangent>& tangentialPoints, sf::RenderWindow& window, float effectRadius);
    void renderUniformFields(std::vector<Uniform>& uniformFields, sf::RenderWindow& window, float width, float height);
};

#endif // __RENDERER_H__