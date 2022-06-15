#ifndef __TANGENT_H__
#define __TANGENT_H__

#include "Point.h"

struct Tangent : Point
{
	float distance_to_drone = 0.f; // distance entre point et drone
	bool isClockwise;

	Tangent();
	Tangent(float x, float y, float z, bool isClockwise);
};
#endif // __TANGENT_H__