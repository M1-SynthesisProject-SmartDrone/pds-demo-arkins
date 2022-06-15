#ifndef __UNIFORM_H__
#define __UNIFORM_H__

#include "Point.h"

struct Uniform : Point {
		float distance_to_drone = 0; // distance entre point et drone
		int wayx = 0;
		int wayy = 0;

		Uniform();
		Uniform(float x, float y, float z, int wayx, int wayy);
};
#endif // __UNIFORM_H__