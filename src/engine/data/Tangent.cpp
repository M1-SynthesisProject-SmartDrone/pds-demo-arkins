#include "Tangent.h"

#include <iostream>

Tangent::Tangent(){
}

Tangent::Tangent(float x, float y, float z, bool isClockwise) :
	Point(x, y, z), isClockwise(isClockwise)
{}