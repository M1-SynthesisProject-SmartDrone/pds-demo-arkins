#ifndef __TANGENT_H__
#define __TANGENT_H__

struct Tangent {
		float x = 0;
		float y = 0;
		float z = 0;
		float distance_to_drone = 0; // distance entre point et drone
		bool isClockwise = true;

		Tangent();
		Tangent(float x, float y, float z, bool isClockwise);
};
#endif // __TANGENT_H__