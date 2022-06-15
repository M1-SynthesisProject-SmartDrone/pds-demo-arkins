#ifndef __POINT_H__
#define __POINT_H__

/**
 * Base struct for all geometrical shapes
 */
struct Point
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    Point();
    Point(float x, float y, float z);
};

#endif // __POINT_H__