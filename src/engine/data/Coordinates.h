#ifndef __COORDINATES_H__
#define __COORDINATES_H__

#include "Point.h"

struct Coordinates : Point {
		float rotation = 0; 
		float attraction = 0;		 // coefficient d'attraction
		float distance_to_drone = 0; // distance entre point et drone

		Coordinates();
		Coordinates(float x, float y, float z, float r);
};
#endif // __COORDINATES_H__