#ifndef ROAD_H_
#define ROAD_H_

#include <vector>
#include <algorithm>
#include <queue>

#include "car.h"
#include "codecraft_util.h"

using std::vector;
using std::priority_queue;

class Road{
 public:
  int id;
  int length;
  int speed_limit;
  int channel;
  int start;
  int end;
  bool bidirectional;

  vector<vector<int>> lane;
  priority_queue<int> lane_wait;
  int lane_last_pos;
  int lane_last_time;

  vector<vector<int>> lane1;
  priority_queue<int> lane1_wait;
  int lane1_last_pos;
  int lane1_last_time;

  int cars_num_road;
  int NewCarTime(int car_id);
  bool AddCar(vector<Car>& cars, int car_id, int direction);
  void RoadRun(vector<Car>& cars, priority_queue<int> lane_wait);
  void RoadRunLane(vector<Car>& cars, vector<vector<int>>& lane, priority_queue<int> lane_wait);
};

#endif //ROAD_H_
