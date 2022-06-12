#include "Renderer.h"

#include <algorithm>
#include <iostream>

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
    // Init drone
    m_droneTexture = loadTexture(DRONE_IMAGE_NAME);
    m_droneSprite.setTexture(&m_droneTexture);
    // Size of the drone depends on the window size
    float droneWidth = m_windowConfig.width * DRONE_SIZE_RATIO;
    float droneHeight = m_windowConfig.height * DRONE_SIZE_RATIO;
    m_droneSprite.setSize(sf::Vector2f(droneWidth, droneHeight));
    m_droneSprite.setOrigin(sf::Vector2f(droneWidth / 2, droneHeight / 2));

    // Pause icon
    m_pauseTexture = loadTexture(PAUSE_IMAGE_NAME);
    m_pauseSprite.setTexture(&m_pauseTexture);
    // This is a square
    float pauseWidth = m_windowConfig.width * PAUSE_SIZE_RATIO;
    m_pauseSprite.setSize(sf::Vector2f(pauseWidth, pauseWidth));
    // Position is fixed
    m_pauseSprite.setPosition(0.0f, 0.0f);

    // Precalculation of any point radius
    m_pointRadius = min(m_windowConfig.width, m_windowConfig.height) * 0.025f;
}

Renderer::~Renderer()
{}

sf::Texture Renderer::loadTexture(string baseFilename)
{
    fs::path baseFilePath(baseFilename);
    auto fullPath = m_imageFolderPath / baseFilePath;
    sf::Texture texture;
    if (!texture.loadFromFile(fullPath.string()))
    {
        stringstream ss;
        ss << "Error while loading texture on path " << fullPath;
        throw runtime_error(ss.str());
    }
    texture.setSmooth(m_imageConfig.enableSmooth);
    return texture;
}

void Renderer::renderGrid(sf::RenderWindow& window)
{
    float midGridThickness = GRID_THICKNESS / 2.0f;

    sf::RectangleShape lineX(sf::Vector2f(m_windowConfig.width, GRID_THICKNESS));
    sf::RectangleShape lineY(sf::Vector2f(GRID_THICKNESS, m_windowConfig.height));

    lineX.setFillColor(GRID_COLOR);
    lineY.setFillColor(GRID_COLOR);

    lineY.setPosition((m_windowConfig.width / 2) - midGridThickness, 0.0f);
    lineX.setPosition(0.0f, (m_windowConfig.height / 2) - midGridThickness);
    window.draw(lineX);
    window.draw(lineY);
}

void Renderer::renderMisc(sf::RenderWindow& window, bool isPaused)
{
    if (isPaused)
    {
        window.draw(m_pauseSprite);
    }
}

void Renderer::renderDrone(Coordinates& droneCoordinates, sf::RenderWindow& window)
{
    m_droneSprite.setPosition(calculatePos(droneCoordinates));
    m_droneSprite.setRotation(droneCoordinates.rotation);
    window.draw(m_droneSprite);
}

void Renderer::renderbarycenter(Coordinates& barycenter, sf::RenderWindow& window)
{
    auto point = createCircle(m_pointRadius, BARYCENTER_POINT_COLOR);
    point.setPosition(calculatePos(barycenter));
    window.draw(point);
}

void Renderer::renderAttractivePoints(std::vector<Coordinates>& attractivePoints, sf::RenderWindow& window, float radius)
{
    auto point = createCircle(m_pointRadius, ATTRACTIVE_POINT_COLOR);
    auto range = createCircle(radius, ATTRACTIVE_POINT_COLOR, true);
    for (const auto& c : attractivePoints)
    {
        auto pos = calculatePos(c);
        point.setPosition(pos);
        range.setPosition(pos);
        window.draw(point);
        window.draw(range);
    }
}

void Renderer::renderRepulsivePoints(std::vector<Coordinates>& repulsivePoints, sf::RenderWindow& window, float effectRadius)
{
    auto point = createCircle(m_pointRadius, REPULSIVE_POINT_COLOR);
    // We add another circle to show the effect radius
    auto effectRadiusShape = createCircle(effectRadius, REPULSIVE_POINT_COLOR, true);
    for (const auto& c : repulsivePoints)
    {
        auto pos = calculatePos(c);
        point.setPosition(pos);
        effectRadiusShape.setPosition(pos);
        window.draw(point);
        window.draw(effectRadiusShape);
    }
}

void Renderer::renderUniformFields(std::vector<Coordinates>& uniformFields, sf::RenderWindow& window, float width, float height)
{
    auto rectangle = createRectangle(width, height, UNIFORM_FIELD_COLOR);
    for(const auto& field : uniformFields)
    {
        rectangle.setPosition(field.x, field.y);
        window.draw(rectangle);
    }
}

void Renderer::renderTangentialPoints(std::vector<Coordinates>& tangentialPoints, sf::RenderWindow& window, float effectRadius)
{
    auto point = createCircle(m_pointRadius, TANGENTIAL_POINT_COLOR);
    // We add another circle to show the effect radius
    auto effectRadiusShape = createCircle(effectRadius, TANGENTIAL_POINT_COLOR, true);
    for (const auto& c : tangentialPoints)
    {
        auto pos = calculatePos(c);
        point.setPosition(pos);
        effectRadiusShape.setPosition(pos);
        window.draw(point);
        window.draw(effectRadiusShape);
    }
}

float Renderer::calculateXPos(float x)
{
    auto& m = m_mapInfos;
    float distRatio = (x - m.minX) / m.width;
    return (distRatio * m_windowConfig.width);
}

float Renderer::calculateYPos(float y)
{
    auto& m = m_mapInfos;
    float distRatio = (y - m.minY) / m.height;
    return (distRatio * m_windowConfig.height);
}

sf::Vector2f Renderer::calculatePos(const Coordinates& coordinates)
{
    return {
        calculateXPos(coordinates.x),
        calculateYPos(coordinates.y)
    };
}

float Renderer::calculateRadius(float r)
{
    auto& m = m_mapInfos;
    return (r / m.width) * m_windowConfig.width;
}

sf::CircleShape Renderer::createCircle(float radius, const sf::Color& color, bool colorOutline)
{
    float displayedRadius = calculateRadius(radius);
    sf::CircleShape circle(displayedRadius);
    if (colorOutline)
    {
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(color);
        circle.setOutlineThickness(1.0f);
    }
    else
    {
        circle.setFillColor(color);
    }
    circle.setOrigin(displayedRadius, displayedRadius);
    return circle;
}

sf::RectangleShape Renderer::createRectangle(float width, float height, const sf::Color& color)
{
    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    rectangle.setOutlineColor(color);
    rectangle.setOutlineThickness(2.0f);
    rectangle.setOrigin(sf::Vector2f(width / 2, height / 2));
    return rectangle;
}