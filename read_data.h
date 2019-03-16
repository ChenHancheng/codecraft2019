#ifndef READDATA_H
#define READDATA_H

#include <vector>
#include <string>

#include "road.h"
#include "car.h"
#include "cross.h"

using std::string;
using std::vector;

bool ReadRoad(const string& road_path, vector<Road>& roads);
bool ReadCross(const string& cross_path, vector<Cross>& cross);
bool ReadCar(const string& car_path, vector<Car>& cars);

#endif //READDATA_H
