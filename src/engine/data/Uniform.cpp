#include "Uniform.h"

#include <stdexcept>

Uniform::Uniform(){
}

Uniform::Uniform(float x, float y, float z, int wayx, int wayy) :
	Point(x, y, z), wayx(wayx), wayy(wayy)
{
	if (wayx == 0.f && wayy == 0.f)
	{
		throw std::runtime_error("Uniform instance set with wayx and wayy both set to zero (this is invalid)");
	}
}