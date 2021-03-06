#include "event_manager.h"

void updateEvents(sf::RenderWindow& window, Events& events)
{
    events.reset();
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Close the window (CTRL-C or clicking the button)
        if (event.type == sf::Event::Closed)
        {
            events.isWindowClosed = true;
        }
        // Keyboard one button press (KeyPressed is checked each frame)
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
            case sf::Keyboard::P:
                events.pausePressed = true;
                break;
            case sf::Keyboard::R:
                events.resetPressed = true;
                break;
            case sf::Keyboard::Up:
                events.upPressed = true;
                break;
            case sf::Keyboard::Down:
                events.downPressed = true;
                break;
            default:
                break;
            }
        }
    }
}
