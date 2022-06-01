
#include <loguru/loguru.hpp>
#include <SFML/Graphics.hpp>

#include <config/ConfigParser.h>

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
    
    const float SPEED = width / 10.0f;

    // Create the window, set the framerate and center it
    sf::RenderWindow window(sf::VideoMode(width, height), windowName);
    window.setFramerateLimit(framerate);
    window.setPosition(sf::Vector2i(
        (sf::VideoMode::getDesktopMode().width/2) - (width / 2),
        (sf::VideoMode::getDesktopMode().height/2) - (height / 2)
    ));
    
    // Base rectangle, put at left center of the screen
    sf::RectangleShape rectangle(sf::Vector2f(width / 4, height / 4));
    rectangle.setFillColor(sf::Color::Blue);
    rectangle.setPosition(0.0f, (height / 2.0f) - (rectangle.getSize().y / 2.0f));

    // Background color
    sf::Color bgColor = sf::Color::Black;

    // Main loop
    bool isPaused = false;
    while(window.isOpen())
    {
        // ==== events (key, window, etc) ====
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close the window (CTRL-C or clicking the button)
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            // Keyboard one button press (KeyPressed is checked each frame)
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::P)
                {
                    isPaused = !isPaused;
                }
            }
        }

        // ==== Application logic (arkins in our case) ====
        if (!isPaused)
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
