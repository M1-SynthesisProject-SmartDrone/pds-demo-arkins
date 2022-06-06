#ifndef __INFORMATIONS_H__
#define __INFORMATIONS_H__

#include "Vector.h"

struct Informations
{
	float ratioX;
	float ratioY;
	float ratioZ;
	float ratioR;
	bool inRange;
	bool isArrived;
	Vector vector;
	int nearestPointIndex;

	Informations();
	Informations(float ratioX, float ratioY, float ratioZ, float ratioR);
};
#endif // __INFORMATIONS_H__