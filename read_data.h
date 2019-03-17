#ifndef READDATA_H
#define READDATA_H

#include <vector>
#include <string>
#include <unordered_map>

#include "road.h"
#include "car.h"
#include "cross.h"
#include "data_type.h"

using std::string;
using std::vector;
using std::unordered_map;

bool ReadRoad(const string& road_path, vector<RoadData>& roads_data);
bool ReadCross(const string& cross_path,vector<CrossData>& cross_data);
bool ReadCar(const string& car_path, vector<CarData>& cars_data);

void ReadData(const string& car_path, const string& cross_path, const string& road_path,
    vector<Car>& cars, vector<Cross>& crosses, vector<Road>& roads, unordered_map<int, vector<int>> ready_cars);

#endif //READDATA_H
