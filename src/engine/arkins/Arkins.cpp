#include "Arkins.h"

Arkins::Arkins()
{

}

Arkins::Arkins(std::vector<Coordinates> attractionPoints, std::vector<Coordinates> repulsionPoints,
	std::vector<Coordinates> tangentialPoints) : attractionPoints(attractionPoints),
	repulsionPoints(repulsionPoints), tangentialPoints(tangentialPoints)
{}

Informations& Arkins::getInfos()
{
	return infos;
}

int Arkins::countAttractionPoints()
{
	return attractionPoints.size();
}

std::vector<Coordinates>& Arkins::getAttractionPoints()
{
	return attractionPoints;
}

std::vector<Coordinates>& Arkins::getRepulsionPoints()
{
	return repulsionPoints;
}

std::vector<Coordinates>& Arkins::getTangentialPoints()
{
	return tangentialPoints;
}

void Arkins::process(Coordinates& droneCoordinates)
{
	for (int attPoints = 0; attPoints < attractionPoints.size(); attPoints++)
	{
		calculate_dist_between_points(droneCoordinates, attractionPoints.at(attPoints));
	}

	Coordinates maxPoint = findMax(attractionPoints);

	calculate_coefficient_attraction(attractionPoints, maxPoint.distance_to_drone);
	Coordinates barycenter = calculate_barycenter(attractionPoints);
	calculate_ratios(droneCoordinates, infos, barycenter);
}

void Arkins::deleteAttractivePoint()
{
	attractionPoints.erase(attractionPoints.begin());
}

void Arkins::resetAttractivePoints(std::vector<Coordinates> attractionPoints)
{
	this->attractionPoints = attractionPoints;
}

Coordinates Arkins::findMax(std::vector<Coordinates>& vector)
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

Coordinates Arkins::calculate_barycenter(std::vector<Coordinates>& vector)
{
	Coordinates barycenter;
	float sum_x = 0;
	float sum_y = 0;
	float sum_z = 0;
	float sum_coef = 0;
	for (const auto& point : vector)
	{
		sum_x += point.attraction * point.x;
		sum_y += point.attraction * point.y;
		sum_z += point.attraction * point.z;
		sum_coef += point.attraction;
	}
	LOG_F(INFO, "zqduqbz (But) : px: %f | py: %f | pz: %f | coef : %f", sum_x, sum_y, sum_z, sum_coef);
	barycenter.x = sum_x / sum_coef;
	barycenter.y = sum_y / sum_coef;
	barycenter.z = sum_z / sum_coef;
	barycenter.attraction = 1;
	return barycenter;
}

void Arkins::calculate_dist_between_points(Coordinates& droneCoordinates, Coordinates& attractionPoint)
{
	float dist = abs(sqrt(pow((droneCoordinates.x - attractionPoint.x), 2) + pow((droneCoordinates.y - attractionPoint.y), 2) + pow((droneCoordinates.z - attractionPoint.z), 2)));
	attractionPoint.distance_to_drone = dist;
}

void Arkins::calculate_coefficient_attraction(std::vector<Coordinates>& vector, float maxDistance)
{
	if (vector.size() == 1)
	{
		vector[0].attraction = 1.0f;
	}
	else
	{
		for (auto& coordinate : vector)
		{
			float coef = 1.0f - coordinate.distance_to_drone / maxDistance;
			// + on est proche du point d'attraction, + le coefficient sera grand [d'où le 1 - rapport distance/maxDistance]
			coordinate.attraction = coef;
		}
	}
}

void Arkins::calculate_vector(Coordinates& droneCoordinates, Coordinates& targetCoordinates, float& x, float& y, float& z)
{
	x = droneCoordinates.x - targetCoordinates.x;
	y = droneCoordinates.y - targetCoordinates.y;
	z = droneCoordinates.z - droneCoordinates.z;
}

bool Arkins::isInRepulsionRadius(Coordinates& droneCoordinates, Coordinates& repulsionPoint)
{
	if (droneCoordinates.x <= repulsionPoint.x + REPULSION_RADIUS && droneCoordinates.x >= repulsionPoint.x - REPULSION_RADIUS)
	{
		if (droneCoordinates.y <= repulsionPoint.y + REPULSION_RADIUS && droneCoordinates.z >= repulsionPoint.z - REPULSION_RADIUS)
		{
			return true;
		}
	}
	return false;
}

void Arkins::calculate_rotation(float hdg, float targeted_hdg, float& r)
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
	if (p == b) return 0.0f;
	else if (p < b) return 1.0f;
	else return -1.0f;
}

void Arkins::calculate_ratios(Coordinates& droneCoordinates, Informations& infos, Coordinates& barycenter)
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

	infos.inRange = (px > bx - RANGE && px < bx + RANGE)
		&& (py > by - RANGE && py < by + RANGE)
		&& (pz > bz - RANGE && pz < bz + RANGE);

	// On effectue une comparaison afin de savoir dans quelle direction on doit se diriger en x, y et z
	if (infos.inRange)
	{
		// On alerte qu'on est entré dans la sphère d'arrivée, les ratios sont définies par défaut à 0
		bool onPoint = (px == bx) && (py == by) && (pz == bz);
		if (onPoint)
		{
			infos.isArrived = true;
			return;
		}
	}

	infos.inRange = false;
	infos.isArrived = false;
	// LOG_F(INFO, "Not on the point yet.");
	calculate_vector(droneCoordinates, barycenter, infos.vector.vx, infos.vector.vy, infos.vector.vz);
	calculate_rotation(droneCoordinates.rotation, barycenter.rotation, infos.vector.vr);

	//LOG_F(INFO, "Informations 4 (Translated vector): vx: %f | vy: %f | vz: %f | vr: %f", infos.vector.vx, infos.vector.vy, infos.vector.vz, infos.vector.vr);

	infos.ratioX = findRatio(px, bx);
	infos.ratioY = findRatio(py, by);
	infos.ratioZ = findRatio(pz, bz);

	LOG_F(INFO, "Informations 5 (info struct): rx = %f | ry = %f | rz = %f | rr = %f | inRange = %d | isArrived = %d", infos.ratioX, infos.ratioY, infos.ratioZ, infos.ratioR, infos.inRange, infos.isArrived);
}