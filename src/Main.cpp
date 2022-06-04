
#include <loguru/loguru.hpp>
#include <SFML/Graphics.hpp>

#include "config/ConfigParser.h"
#include "gui/events/event_manager.h"

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

    const float SPEED = width / 10.0f;
    
    // Base rectangle, put at left center of the screen
    sf::RectangleShape rectangle(sf::Vector2f(width / 4, height / 4));
    rectangle.setFillColor(sf::Color::Blue);
    rectangle.setPosition(0.0f, (height / 2.0f) - (rectangle.getSize().y / 2.0f));

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
            rectangle.move(1.0f, 0.0f);
        }
        
        // ==== Rendering ====
        window.clear(bgColor);
        // Draw entities here
        window.draw(rectangle);
        // End rendering
        // As we defined framerate, we will have some sleeps calls after display
        window.display();
    }

    return 0;
}
