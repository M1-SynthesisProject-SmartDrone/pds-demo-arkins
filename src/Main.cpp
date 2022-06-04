
#include <loguru/loguru.hpp>
#include <SFML/Graphics.hpp>

#include "config/ConfigParser.h"
#include "engine/map/MapInfos.h"
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

    MapInfos mapInfos{0.0, 2000.0, 0.0, 2000.0};

    Renderer renderer(params.image, params.window, mapInfos);

    Coordinates droneCoordinates(100.0f, 100.0f, 0.0f, 0.0f);

    // TODO only for drawing test
    vector<Coordinates> attractivePoints{
        Coordinates{500.0, 500.0, 0.0, 0.0},
        Coordinates{200.0, 200.0, 0.0, 0.0},
    };

    vector<Coordinates> repulsivePoints{
        Coordinates{700.0, 350.0, 0.0, 0.0},
        Coordinates{1000.0, 1000.0, 0.0, 0.0},
    };

    vector<Coordinates> tangentialPoints{
        Coordinates{1950.0, 1650.0, 0.0, 0.0},
        Coordinates{222.0, 333.0, 0.0, 0.0},
    };


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

        // Render drone at the end, so that it is on top of all others
        renderer.renderDrone(droneCoordinates, window);

        // End rendering
        // As we defined framerate, we will have some sleeps calls after display
        window.display();
    }

    return 0;
}
