#include "Arkins.h"

Arkins::Arkins()
{
}

Arkins::Arkins(std::vector<Coordinates> attractionPoints, std::vector<Coordinates> repulsionPoints,
			   std::vector<Coordinates> tangentialPoints, std::vector<Coordinates> uniformPoints) : attractionPoints(attractionPoints),
																									repulsionPoints(repulsionPoints), tangentialPoints(tangentialPoints), uniformPoints(uniformPoints)
{
}

Informations &Arkins::getInfos()
{
	return infos;
}

int Arkins::countAttractionPoints()
{
	return attractionPoints.size();
}

std::vector<Coordinates> &Arkins::getAttractionPoints()
{
	return attractionPoints;
}

std::vector<Coordinates> &Arkins::getRepulsionPoints()
{
	return repulsionPoints;
}

std::vector<Coordinates> &Arkins::getTangentialPoints()
{
	return tangentialPoints;
}

std::vector<Coordinates> &Arkins::getUniformPoints()
{
	return uniformPoints;
}
void Arkins::process(Coordinates &droneCoordinates)
{
	auto start = chrono::steady_clock::now();
	bool isInRepulsivePoint = false;
	bool isInUniformPoint = false;
	for (Coordinates &repulsivePoint : repulsionPoints)
	{
		calculate_dist_between_points(droneCoordinates, repulsivePoint);
		if (isInRepulsionRadius(droneCoordinates, repulsivePoint))
		{
			isInRepulsivePoint = true;
		}
	}

	for (Coordinates &uniformPoint : uniformPoints)
	{
		LOG_F(ERROR, "Informations Drone: px: %f | py: %f | pz: %f", droneCoordinates.x, droneCoordinates.y, droneCoordinates.z);
		LOG_F(ERROR, "Informations Uniform: px: %f | py: %f | pz: %f", uniformPoint.x, uniformPoint.y, uniformPoint.z);
		if (isInUniformRadius(droneCoordinates, uniformPoint))
		{
			LOG_F(ERROR, "IT IS IN UNIFORM !!");

			isInUniformPoint = true;
		}
	}

	int minPointIdx = -1;
	float minDistance = 99999.0f;
	for (int attPoints = 0; attPoints < attractionPoints.size(); attPoints++)
	{
		auto &point = attractionPoints.at(attPoints);
		calculate_dist_between_points(droneCoordinates, point);
		if (point.distance_to_drone < minDistance)
		{
			minDistance = point.distance_to_drone;
			minPointIdx = attPoints;
		}
	}
	infos.nearestPointIndex = minPointIdx;

	auto &point = attractionPoints.at(minPointIdx);
	infos.inRange = isInAttractiveRange(droneCoordinates, point);

	Coordinates maxPoint = findMax(attractionPoints);

	calculate_coefficient_attraction(attractionPoints, maxPoint.distance_to_drone);
	Coordinates barycenter = calculate_barycenter(attractionPoints, infos);

	if (isInRepulsivePoint)
	{
		for (Coordinates &repulsivePoint : repulsionPoints)
		{
			// LOG_F(ERROR, "Distance between r et d: %f | RANGE = %f", repulsivePoint.distance_to_drone, REPULSION_RADIUS);
			if (isInRepulsionRadius(droneCoordinates, repulsivePoint))
			{
				repulsion(droneCoordinates, barycenter, repulsivePoint, infos);
			}
		}
	}
	else if (isInUniformPoint)
	{
		for (Coordinates &uniformPoint : uniformPoints)
		{
			uniform(droneCoordinates, uniformPoint, barycenter, infos);
		}
	}
	else
	{
		calculate_ratios(droneCoordinates, infos, barycenter);
	}

	auto end = chrono::steady_clock::now();

	LOG_F(WARNING, "ELAPSED TIME FOR PROCESSING : %ld µs", chrono::duration_cast<chrono::microseconds>(end - start).count());
}

bool Arkins::isInAttractiveRange(Coordinates &droneCoordinates, Coordinates &attractivePoint)
{
	// Find if the nearest point is in range (i.e, to be deleted)
	float px = droneCoordinates.x;
	float py = droneCoordinates.y;
	float pz = droneCoordinates.z;

	float bx = attractivePoint.x;
	float by = attractivePoint.y;
	float bz = attractivePoint.z;

	return (px > bx - RANGE && px < bx + RANGE) && (py > by - RANGE && py < by + RANGE) && (pz > bz - RANGE && pz < bz + RANGE);
}

void Arkins::deleteAttractivePoint()
{
	attractionPoints.erase(attractionPoints.begin());
}

void Arkins::deleteAttractivePoint(int index)
{
	attractionPoints.erase(attractionPoints.begin() + index);
}

void Arkins::resetAttractivePoints(std::vector<Coordinates> attractionPoints)
{
	this->attractionPoints = attractionPoints;
}

Coordinates Arkins::findMax(std::vector<Coordinates> &vector)
{
	int max = 0;
	Coordinates maxPoint;
	for (int index = 0; index < vector.size(); index++)
	{
		Coordinates point = vector.at(index);
		if (max < point.distance_to_drone)
		{
			max = point.distance_to_drone;
			maxPoint = point;
		}
	}
	return maxPoint;
}

Coordinates Arkins::calculate_barycenter(std::vector<Coordinates> &vector, Informations &infos)
{
	Coordinates barycenter;
	float sum_x = 0;
	float sum_y = 0;
	float sum_z = 0;
	float sum_coef = 0;
	int index = 0;
	for (const auto &point : vector)
	{
		// LOG_F(WARNING, "ATTRACTION n°%d : x:%f y:%f z:%f",index, point.x, point.y, point.z);
		sum_x += point.attraction * point.x;
		sum_y += point.attraction * point.y;
		sum_z += point.attraction * point.z;
		sum_coef += point.attraction;
		index = index + 1;
	}
	// LOG_F(INFO, "Informations (But) : px: %f | py: %f | pz: %f | coef : %f", sum_x, sum_y, sum_z, sum_coef);
	barycenter.x = sum_x / sum_coef;
	barycenter.y = sum_y / sum_coef;
	barycenter.z = sum_z / sum_coef;
	infos.barycenter = barycenter;

	return barycenter;
}

void Arkins::calculate_dist_between_points(Coordinates &droneCoordinates, Coordinates &attractionPoint)
{
	float dist = abs(sqrt(pow((droneCoordinates.x - attractionPoint.x), 2) + pow((droneCoordinates.y - attractionPoint.y), 2) + pow((droneCoordinates.z - attractionPoint.z), 2)));
	attractionPoint.distance_to_drone = dist;
}

void Arkins::calculate_coefficient_attraction(std::vector<Coordinates> &vector, float maxDistance)
{
	if (vector.size() == 1)
	{
		vector[0].attraction = 1.0f;
	}
	else
	{
		for (auto &coordinate : vector)
		{
			float coef = exp(exp(exp(1.0f - coordinate.distance_to_drone / maxDistance)));
			// + on est proche du point d'attraction, + le coefficient sera grand [d'où le 1 - rapport distance/maxDistance]
			coordinate.attraction = coef;
		}
	}
}

void Arkins::calculate_rotation(float hdg, float targeted_hdg, float &r)
{
	float theta = abs(hdg - targeted_hdg);

	if (theta < 180)
	{
		r = -1;
	}
	else
	{
		r = 1;
	}
}

float findRatio(float p, float b)
{
	if (p == b)
		return 0.0f;
	else if (p < b)
		return 1.0f;
	else
		return -1.0f;
}

void Arkins::calculate_ratios(Coordinates &droneCoordinates, Informations &infos, Coordinates &barycenter)
{
	/*
		Ce qui sera retourné
		infos : x, y, z, r (entre -1 et 1), inRange [disant si le drone est a portée] et isArrived [disant si le drone est bien arrivé]
	*/
	// Coordonnées cartésiennes du drone
	float px = droneCoordinates.x;
	float py = droneCoordinates.y;
	float pz = droneCoordinates.z;

	// Coordonées cartésiennes du point de destination
	float bx = barycenter.x;
	float by = barycenter.y;
	float bz = barycenter.z;

	LOG_F(INFO, "Informations 2 (Drone cartesian): px: %f | py: %f | pz: %f", px, py, pz);
	LOG_F(INFO, "Informations 3 (But) : bx: %f | by: %f | bz: %f", bx, by, bz);

	calculate_vector(droneCoordinates, barycenter, infos.vector.vx, infos.vector.vy, infos.vector.vz);
	calculate_rotation(droneCoordinates.rotation, barycenter.rotation, infos.vector.vr);

	// LOG_F(INFO, "Informations 4 (Translated vector): vx: %f | vy: %f | vz: %f | vr: %f", infos.vector.vx, infos.vector.vy, infos.vector.vz, infos.vector.vr);

	infos.ratioX = findRatio(px, bx);
	infos.ratioY = findRatio(py, by);
	infos.ratioZ = findRatio(pz, bz);

	// LOG_F(INFO, "Informations 5 (info struct): rx = %f | ry = %f | rz = %f | rr = %f | inRange = %d | isArrived = %d", infos.ratioX, infos.ratioY, infos.ratioZ, infos.ratioR, infos.inRange, infos.isArrived);
}

bool Arkins::isInRepulsionRadius(Coordinates &droneCoordinates, Coordinates &repulsionPoint)
{
	return repulsionPoint.distance_to_drone <= REPULSION_RADIUS;
}

bool Arkins::isInUniformRadius(Coordinates &droneCoordinates, Coordinates &uniformRadius)
{
	return droneCoordinates.x >= (uniformRadius.x - UNIFORM_WIDTH / 2) && droneCoordinates.x <= (uniformRadius.x + UNIFORM_WIDTH / 2) && droneCoordinates.y >= (uniformRadius.y - UNIFORM_HEIGHT / 2) && droneCoordinates.y <= (uniformRadius.y + UNIFORM_HEIGHT / 2);
}

void Arkins::calculate_vector(Coordinates &droneCoordinates, Coordinates &targetCoordinates, float &x, float &y, float &z)
{
	x = droneCoordinates.x - targetCoordinates.x;
	y = droneCoordinates.y - targetCoordinates.y;
	z = droneCoordinates.z - droneCoordinates.z;
}

void Arkins::calculate_vector(Coordinates &droneCoordinates, Coordinates &targetCoordinates, Vector &vector)
{
	vector.vx = droneCoordinates.x - targetCoordinates.x;
	vector.vy = droneCoordinates.y - targetCoordinates.y;
	vector.vz = droneCoordinates.z - droneCoordinates.z;
}

void Arkins::calculate_vector(Vector &vect1, Vector &vect2, Vector &vector)
{
	vector.vx = vect1.vx + vect2.vx;
	vector.vy = vect1.vy + vect2.vy;
	vector.vz = vect1.vz + vect2.vz;
	// LOG_F(ERROR, "VECTEUR INSIDE : %f %f %f", vector.vx, vector.vy, vector.vy);
}

void Arkins::repulsion(Coordinates &droneCoordinates, Coordinates &barycenter, Coordinates &repulsivePoint, Informations &infos)
{
	Vector rd;
	Vector db;
	Vector g;
	calculate_vector(droneCoordinates, repulsivePoint, rd);
	calculate_vector(barycenter, droneCoordinates, db);
	calculate_vector(rd, db, g);
	infos.vector = g;
	infos.ratioX = findRatio(droneCoordinates.x, droneCoordinates.x + g.vx);
	infos.ratioY = findRatio(droneCoordinates.y, droneCoordinates.y + g.vy);
	infos.ratioZ = findRatio(droneCoordinates.z, droneCoordinates.z + g.vz);
	// LOG_F(ERROR, " rdvx:%f rdvy:%f rdvz:%f | dbvx:%f dbvy:%f dbvz:%f| gvx:%f gvy:%f gvz:%f | irx:%f iry:%f irz:%f ",
	//		rd.vx, rd.vy, rd.vz, db.vx, db.vy, db.vz, g.vx, g.vy, g.vz, infos.ratioX, infos.ratioY, infos.ratioZ);
}

void Arkins::uniform(Coordinates &droneCoordinates, Coordinates &uniformPoint, Coordinates &goalPoint, Informations &infos)
{
	Vector ud;
	Vector db;
	Vector g;
	calculate_vector(droneCoordinates, uniformPoint, ud);
	calculate_vector(droneCoordinates, goalPoint, db);
	calculate_vector(ud, db, g);
	infos.vector = g;
	infos.ratioX = findRatio(droneCoordinates.x, droneCoordinates.x + g.vx);
	infos.ratioY = findRatio(droneCoordinates.y, droneCoordinates.y + g.vy);
	infos.ratioZ = findRatio(droneCoordinates.z, droneCoordinates.z + g.vz);
}