#ifndef DISPATCH_h_
#define DISPATCH_h_

#include <vector>
#include <utility>
#include <unordered_map>
#include <iomanip>
#include <queue>
#include <iostream>

#include "road.h"
#include "cross.h"
#include "car.h"
#include "dijkstra.h"

using std::priority_queue;
using std::unordered_map;
using std::pair;
using std::vector;
using std::min;

// void RecordSolution(vector<Car>& cars, vector<Car>& cars_tmp, vector<Road>& roads, vector<Road>& road_tmp);
void RecordSolution(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses);
void LaunchCar(int T_count, vector<Car>& cars, vector<Cross>& crosses, unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>>& ready_car, vector<Road>& roads, Graph& graph);
void Dispatch(vector<Car>& cars, vector<Road>& roads, vector<Cross>& crosses, Graph& graph, unordered_map<int, priority_queue<int, vector<int>, std::greater<int>>> ready_cars);


#endif //DISPATCH_h_