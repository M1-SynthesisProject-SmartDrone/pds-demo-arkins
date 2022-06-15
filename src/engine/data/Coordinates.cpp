#include "Coordinates.h"

Coordinates::Coordinates(){
}

Coordinates::Coordinates(float x, float y, float z, float r) :
	Point(x, y, z), rotation(r)
{}