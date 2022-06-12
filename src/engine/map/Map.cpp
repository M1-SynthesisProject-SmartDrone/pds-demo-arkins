#include "Map.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>
#include <loguru/loguru.hpp>

using namespace std;
namespace fs = std::filesystem;

nlohmann::json loadJson(const std::string& filename);
Coordinates findDroneCoordinates(const nlohmann::json& json);
vector<Coordinates> findPoints(const nlohmann::json& json, const char* fieldName);

// Update the MapInfos struct if new min or max values are found
void updateBoundaries(const Coordinates& coordinates, MapInfos& infos);
void updateBoundaries(const vector<Coordinates>& points, MapInfos& infos);

Map::Map(ConfigMap mapConfig, ConfigWindow windowConfig)
{
    LOG_F(INFO, "Load json file %s", mapConfig.filename.c_str());
    auto json = loadJson(mapConfig.filename);

    droneCoordinates = findDroneCoordinates(json);
    attractivePoints = findPoints(json, "attractivePoints");
    repulsivePoints = findPoints(json, "repulsivePoints");
    tangentialPoints = findPoints(json, "tangentialPoints");
    uniformFields = findPoints(json, "uniformFields");

    LOG_F(INFO, "Create map infos");
    // Create map infos
    // We must go through each point in order to find min and max coordinates
    // First, init the map infos values so that it updates accordingly
    infos.minX = droneCoordinates.x;
    infos.maxX = droneCoordinates.x;
    infos.minY = droneCoordinates.y;
    infos.maxY = droneCoordinates.y;
    updateBoundaries(attractivePoints, infos);
    updateBoundaries(repulsivePoints, infos);
    updateBoundaries(tangentialPoints, infos);
    updateBoundaries(uniformFields, infos);

    // Add space arround
    infos.minX -= mapConfig.spaceArround;
    infos.maxX += mapConfig.spaceArround;
    infos.minY -= mapConfig.spaceArround;
    infos.maxY += mapConfig.spaceArround;

    infos.width = infos.maxX - infos.minX;
    infos.height = infos.maxY - infos.minY;

    printf("Map width : %f, map height : %f\n", infos.width, infos.height);

    // We want to have the same ratio as the window we will display on
    float mapRatio = infos.width / infos.height;
    float windowRatio = (float) windowConfig.width / (float) windowConfig.height;
    printf("Map ratio : %f, Window ratio : %f\n", mapRatio, windowRatio);
    if (mapRatio > windowRatio)
    {
        // map height is not big enough
        float newMapHeight = infos.width / windowRatio;
        float offset = (newMapHeight - infos.height) / 2;
        infos.minY -= offset;
        infos.maxY += offset;
        infos.height = newMapHeight;
    }
    else if (mapRatio < windowRatio)
    {
        // map width is not big enough
        float newMapWidth = infos.height * windowRatio;
        float offset = (newMapWidth - infos.width) / 2;
        infos.minX -= offset;
        infos.maxX += offset;
        infos.width = newMapWidth;
    }
    printf("Map width : %f, map height : %f\n", infos.width, infos.height);
}

Map::~Map()
{}

nlohmann::json loadJson(const std::string& filename)
{
    if (!fs::exists(filename))
    {
        stringstream ss;
        ss << "The file \"" << filename << "\" does not exists";
        throw runtime_error(ss.str());
    }
    ifstream inputStream(filename);
    return nlohmann::json::parse(inputStream);
}

Coordinates findDroneCoordinates(const nlohmann::json& json)
{
    auto droneJson = json["drone"];
    return Coordinates(
        droneJson["x"],
        droneJson["y"],
        0.0f,
        droneJson["rotation"]
    );
}

vector<Coordinates> findPoints(const nlohmann::json& json, const char* fieldName)
{
    auto jsonArray = json[fieldName];
    vector<Coordinates> points;
    points.reserve(jsonArray.size());

    for (const auto& j : jsonArray)
    {
        points.push_back(Coordinates(
            j["x"],
            j["y"],
            0.0f,
            0.0f
        ));
    }
    return points;
}

void updateBoundaries(const Coordinates& coordinates, MapInfos& infos)
{
    auto x = coordinates.x;
    auto y = coordinates.y;

    if (x < infos.minX)
    {
        infos.minX = x;
    }
    else if (x > infos.maxX)
    {
        infos.maxX = x;
    }
    else if (y < infos.minY)
    {
        infos.minY = y;
    }
    else if (y > infos.maxY)
    {
        infos.maxY = y;
    }
}

void updateBoundaries(const vector<Coordinates>& points, MapInfos& infos)
{
    for (const auto& p : points)
    {
        updateBoundaries(p, infos);
    }
}