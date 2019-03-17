#ifndef ROAD_H_
#define ROAD_H_

#include <algorithm>
#include <queue>
#include <vector>

#include "car.h"
#include "codecraft_util.h"
#include "data_type.h"

using std::priority_queue;
using std::vector;

class Road {
 public:
  int id;
  int length;
  int speed_limit;
  int channel;
  int start;
  int end;
  bool bidirectional;

  Road() = default;
  Road(const RoadData& road_data)
      : id(road_data.id),
        length(road_data.length),
        speed_limit(road_data.speed_limit),
        channel(road_data.channel),
        start(road_data.start),
        end(road_data.end),
        bidirectional(road_data.bidirectional) {
    lane = vector<vector<int>>(channel, vector<int>(length, 0));
    lane_last_pos = vector<int>(channel, 0);
    lane_last_time = vector<int>(channel, 0);
    if(bidirectional){
      lane1= vector<vector<int>>(channel, vector<int>(length, 0));
      lane1_last_pos = vector<int>(channel, 0);
      lane1_last_time = vector<int> (channel, 0);
    }
  }

  vector<vector<int>> lane;
  priority_queue<int> lane_wait;
  vector<int> lane_last_pos;
  vector<int> lane_last_time;

  vector<vector<int>> lane1;
  priority_queue<int> lane1_wait;
  vector<int> lane1_last_pos;
  vector<int> lane1_last_time;
  int cars_num_road;

  int QueryRoadState(int car_id);
  bool AddCar(vector<Car>& cars, int car_id, int direction);
  void RoadRun(vector<Car>& cars);
  void RoadRunLane(vector<Car>& cars, vector<vector<int>>& lane,
                   priority_queue<int> lane_wait);
};

#endif  // ROAD_H_
