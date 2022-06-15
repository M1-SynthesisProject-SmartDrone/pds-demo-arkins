#include "Renderer.h"

#include <algorithm>
#include <iostream>
#include <math.h>

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
    auto scaleX = m_windowConfig.width / m_mapInfos.width;
    auto scaleY = m_windowConfig.height / m_mapInfos.height;

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

    // Precalculation of some values
    m_pointRadius = min(m_windowConfig.width, m_windowConfig.height) * 0.025f;
    m_arrowSize = sf::Vector2f(ARROW_HEAD_SCALE * scaleX, ARROW_HEAD_SCALE * scaleY);
    m_scaleVector = sf::Vector2f(m_windowConfig.width / m_mapInfos.width, m_windowConfig.height / m_mapInfos.height);
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
    m_droneSprite.setPosition(calculatePos(&droneCoordinates));
    m_droneSprite.setRotation(droneCoordinates.rotation);
    window.draw(m_droneSprite);
}

void Renderer::renderbarycenter(Coordinates& barycenter, sf::RenderWindow& window)
{
    auto point = createCircle(m_pointRadius, BARYCENTER_POINT_COLOR);
    point.setPosition(calculatePos(&barycenter));
    window.draw(point);
}

void Renderer::renderAttractivePoints(std::vector<Coordinates>& attractivePoints, sf::RenderWindow& window, float radius)
{
    auto point = createCircle(m_pointRadius, ATTRACTIVE_POINT_COLOR);
    auto range = createCircle(radius, ATTRACTIVE_POINT_COLOR, true);
    for (const auto& c : attractivePoints)
    {
        auto pos = calculatePos(&c);
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
        auto pos = calculatePos(&c);
        point.setPosition(pos);
        effectRadiusShape.setPosition(pos);
        window.draw(point);
        window.draw(effectRadiusShape);
    }
}

void Renderer::renderUniformFields(std::vector<Uniform>& uniformFields, sf::RenderWindow& window, float width, float height)
{
    auto rectangle = createRectangle(width, height, UNIFORM_FIELD_COLOR);
    rectangle.setFillColor(sf::Color::Transparent);
    ArrowShape arrow;
    if (width < height)
    {
        arrow = createArrow(UNIFORM_FIELD_COLOR, width / 2.f * m_scaleVector.x);
    }
    else
    {
        arrow = createArrow(UNIFORM_FIELD_COLOR, height / 2.f * m_scaleVector.y);
    }
    for (const auto& field : uniformFields)
    {
        auto pos = calculatePos(&field);
        rectangle.setPosition(pos);
        arrow.setPosition(pos);
        
        // Find the angle from the vector (using dot product)
        // angle = (v.b) / (|v| * |b|)
        // Here, b is unit vector, b = (0, -1) and |b| = 1
        int vx = field.wayx, vy = field.wayy;
        float vNorm = sqrt(vx * vx + vy * vy);
        float sign = vx < 0 || vy < 0 ? -1.f : 1.f;
        float angleDeg = sign * acos((vy * -1.f) / vNorm) * 180.f / M_PI;
        if (angleDeg > 0.f)
        {
            angleDeg = 360.0f - angleDeg;
        }
        else
        {
            angleDeg = abs(angleDeg);
        }
        arrow.setRotation(angleDeg);

        window.draw(rectangle);
        window.draw(arrow);
    }
}

void Renderer::renderTangentialPoints(std::vector<Tangent>& tangentialPoints, sf::RenderWindow& window, float effectRadius)
{
    auto point = createCircle(m_pointRadius, TANGENTIAL_POINT_COLOR);
    // We add another circle to show the effect radius
    auto effectRadiusShape = createCircle(effectRadius, TANGENTIAL_POINT_COLOR, true);
    // The arrow showing the direction
    auto arrow = createArrow(TANGENTIAL_POINT_COLOR);
    for (const auto& c : tangentialPoints)
    {
        auto pos = calculatePos(&c);
        point.setPosition(pos);

        effectRadiusShape.setPosition(pos);
        
        cout << c.isClockwise << endl;
        auto arrowAngle = c.isClockwise ? 270.f : 90.f;
        arrow.setRotation(arrowAngle);
        arrow.setPosition({ pos.x, pos.y - (effectRadiusShape.getRadius() * effectRadiusShape.getScale().y) });

        window.draw(point);
        window.draw(effectRadiusShape);
        window.draw(arrow);
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

sf::Vector2f Renderer::calculatePos(const Point* point)
{
    return {
        calculateXPos(point->x),
        calculateYPos(point->y)
    };
}

float Renderer::calculateRadius(float r)
{
    auto& m = m_mapInfos;
    return (r / m.width) * m_windowConfig.width;
}

sf::CircleShape Renderer::createCircle(float radius, const sf::Color& color, bool colorOutline)
{
    // float displayedRadius = calculateRadius(radius);
    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    if (colorOutline)
    {
        circle.setScale(m_scaleVector);
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(color);
        circle.setOutlineThickness(CIRCLE_THICKNESS);
    }
    else
    {
        circle.setScale(sf::Vector2f(m_windowConfig.width / m_mapInfos.width, m_windowConfig.width / m_mapInfos.width));
        circle.setFillColor(color);
    }
    return circle;
}

sf::RectangleShape Renderer::createRectangle(float width, float height, const sf::Color& color)
{
    sf::RectangleShape rectangle(sf::Vector2f(width, height));
    rectangle.setOutlineColor(color);
    rectangle.setOutlineThickness(UNIFORM_THICKNESS);
    rectangle.setOrigin(sf::Vector2f(width / 2, height / 2));
    rectangle.setScale(m_scaleVector);
    return rectangle;
}

ArrowShape Renderer::createArrow(const sf::Color& color, float tailLength)
{
    // The head size depends on the window
    if (tailLength <= 0)
    {
        return {
            {0.f, 0.f},
            m_arrowSize,
            {0.f, 0.f},
            color
        };
    }
    else
    {
        return {
            {0.f, 0.f},
            m_arrowSize,
            {UNIFORM_THICKNESS, tailLength},
            color
        };
    }

}
