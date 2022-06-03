#ifndef __EVENTMANAGER_H__
#define __EVENTMANAGER_H__

#include <SFML/Graphics.hpp>

#include "Events.h"

/**
 * Check all events happening during the current frame.
 */
class EventManager
{
private:
    sf::RenderWindow& m_window;    
public:
    EventManager(sf::RenderWindow& window);
    ~EventManager();

    void update(Events& events);
};

#endif // __EVENTMANAGER_H__