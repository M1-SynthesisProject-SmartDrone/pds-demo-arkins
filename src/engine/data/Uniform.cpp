#include "Uniform.h"

Uniform::Uniform(){
}

Uniform::Uniform(float x, float y, float z, int wayx, int wayy) :
	Point(x, y, z), wayx(wayx), wayy(wayy)
{}