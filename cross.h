#ifndef CROSS_H_
#define CROSS_H_

#include <set>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <queue>

#include "car.h"
#include "road.h"
#include "data_type.h"

#define GO_STRAIGHT 0
#define TURN_LEFT 1
#define TURN_RIGHT 2

using std::map;
using std::set;
using std::vector;
using std::pair;
using std::queue;

class Cross{
 public:
  const int id;
  static int reached_cars;
  Cross() = default;
  Cross(const CrossData& cross_data);

  vector<int> dispatch_seq;
  map<pair<int, int>, int> turn_direction;//eg:turn_direction[{road_id0, road_id1}] returns TRUN_LEFT
  map<pair<int, int>, int> turn_road;//eg:turn_road[{road_id0, TURN_LEFT}] returns road_id1
  int valid_roads_num;
  map<int, queue<int>> road_queue;

  void CalCost();
  void InitialValue(vector<Road>& roads, vector<Car>& cars, const vector<vector<int>> cost_matrix);
  int UpdateCar(vector<Road>& roads, vector<Car>& cars, int car_id); 
};

#endif //CROSS_H_
