#ifndef __EVENTS_H__
#define __EVENTS_H__

/**
 * This holds all important keys that are pressed / released
 * on a particular frame
 */
struct Events
{
    // Special case handled by main function
    bool isWindowClosed = false;

    bool pausePressed = false;
    bool resetPressed = false;
    bool upPressed = false;
    bool downPressed = false;

    void reset()
    {
        pausePressed = false;
        resetPressed = false;
        upPressed = false;
        downPressed = false;
    }
};


#endif // __EVENTS_H__