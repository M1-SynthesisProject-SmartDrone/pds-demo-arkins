#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>

struct Vector {
	float vx = 0;
	float vy = 0;
	float vz = 0;
	float vr = 0;
	Vector();
	Vector(float vx, float vy, float vz, float vr);

	Vector inverted()
	{
		return {-vx, -vy, -vz, -vr};
	}

	Vector toUnitVector()
	{
		auto norm = calcNorm();
		return {
			vx / norm,
			vy / norm,
			vz / norm,
			vr / norm
		};
	}

	float calcNorm()
	{
		return sqrt(vx * vx + vy * vy + vz * vz + vr * vr);
	}
};
#endif // __VECTOR_H__