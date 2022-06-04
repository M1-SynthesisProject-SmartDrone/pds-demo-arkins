
#include <iostream>

#include <loguru/loguru.hpp>
#include <SFML/Graphics.hpp>

#include "config/ConfigParser.h"
#include "engine/map/Map.h"
#include "gui/events/event_manager.h"
#include "gui/renderer/Renderer.h"

using namespace std;

int main(int argc, char *argv[])
{
    loguru::init(argc, argv);
    LOG_F(INFO, "Start the 'DemoArkins' application");

    ConfigParser configParser(argc, argv);
    ConfigParams params = configParser.parse();

    auto width = params.window.width;
    auto height = params.window.height;
    auto windowName = params.window.name;
    auto framerate = params.window.framerate;
    sf::RenderWindow window(sf::VideoMode(width, height), windowName);
    window.setFramerateLimit(framerate);
    window.setPosition(sf::Vector2i(
        (sf::VideoMode::getDesktopMode().width/2) - (width / 2),
        (sf::VideoMode::getDesktopMode().height/2) - (height / 2)
    ));
    window.setVerticalSyncEnabled(params.window.enableVsync);

    Map map(params.map);

    // MapInfos infos{-1000.0, 1000.0, -1000.0, 1000.0, 2000.0f, 2000.0f};

    Renderer renderer(params.image, params.window, map.infos);

    // TODO remove this, put in engine
    Coordinates droneCoordinates = map.droneCoordinates;

    vector<Coordinates> attractivePoints = map.attractivePoints;

    vector<Coordinates> repulsivePoints = map.repulsivePoints;

    vector<Coordinates> tangentialPoints = map.tangentialPoints;
    // TODO uniform


    // Background color
    sf::Color bgColor = sf::Color::White;
    Events events;

    // Main loop
    while(window.isOpen())
    {
        updateEvents(window, events);

        if (events.isWindowClosed)
        {
            window.close();
            continue;
        }

        // ==== Application logic (arkins in our case) ====
        if (!events.isPaused)
        {
            // droneCoordinates.x += 1.0;
            // droneCoordinates.y += 1.0;
        }
        
        // ==== Rendering ====
        window.clear(bgColor);
        // Draw entities here
        
        renderer.renderGrid(window);
        renderer.renderAttractivePoints(attractivePoints, window);
        renderer.renderRepulsivePoints(repulsivePoints, window, 100.0);
        renderer.renderTangentialPoints(tangentialPoints, window, 100.0);
        // TODO call render uniform fields

        // Render drone at the end, so that it is on top of all others
        renderer.renderDrone(droneCoordinates, window);

        // End rendering
        // As we defined framerate, we will have some sleeps calls after display
        window.display();
    }

    return 0;
}
