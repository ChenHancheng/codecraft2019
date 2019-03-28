#ifndef ROAD_H_
#define ROAD_H_

#include <algorithm>
#include <queue>
#include <vector>
#include <ostream>
#include <iomanip>

#include "car.h"
#include "codecraft_util.h"
#include "data_type.h"

#define POS_BLANK -1

#define ADD_SUCCESS 0
#define FRONT_CAR_WAIT 1
#define NEXT_ROAD_FULL 2

using std::min;
using std::priority_queue;
using std::vector;

class Road {
 public:
  const int id;
  const int length;
  const int speed_limit;
  const int channel;
  const int start;
  const int end;
  const bool bidirectional;

  Road() = default;
  Road(const RoadData& road_data)
      : id(road_data.id),
        length(road_data.length),
        speed_limit(road_data.speed_limit),
        channel(road_data.channel),
        start(road_data.start),
        end(road_data.end),
        bidirectional(road_data.bidirectional) {
    max_car_num = channel*length;
    lane0 = vector<vector<int>>(channel, vector<int>(length, POS_BLANK));
    lane0_last_car = vector<int>(channel, -1);
    lane0_last_time = vector<int>(channel, -1);
    if (bidirectional) {
      lane1 = vector<vector<int>>(channel, vector<int>(length, POS_BLANK));
      lane1_last_car = vector<int>(channel, -1);
      lane1_last_time = vector<int>(channel, -1);
    }
  }

  vector<vector<int>> lane0;
  vector<int> lane0_last_car;
  vector<int> lane0_last_time;
  int cars_num_road0=0;

  vector<vector<int>> lane1;
  vector<int> lane1_last_car;
  vector<int> lane1_last_time;
  int cars_num_road1=0;

  int max_car_num;

  void RoadRun(vector<Car>& cars);
  void RoadRunLane(vector<Car>& cars, vector<vector<int>>& lane);

  int QueryRoadStateLane(vector<Car>& cars, int car_id,
                         vector<vector<int>>& lane, vector<int>& lane_last_pos,
                         vector<int>& lane_last_time);
  int QueryRoadState(vector<Car>& cars, int car_id, int direction);

  int AddCarLane(vector<Car>& cars, int car_id, vector<vector<int>>& lane,
                 vector<int>& lane_last_pos);
  int AddCar(vector<Car>& cars, int car_id, int direction);

  void UpdateCar(vector<Car>& cars, const int car_id);
  void UpdateCarLane(vector<Car>& cars, const int car_id,
                     vector<vector<int>>& lane);

  void DeleteCar(vector<Car>& cars, int car_id);
  // void DeleteCar(vector<Car>& cars, int car_id);
  // void DeleteCarLane(vector<Car>& cars, int car_id, vector<vector<int>>&
  // lane,
  //                    vector<int>& lane_last_pos, vector<int>&
  //                    lane_last_time);
  void UpdateLastCar(const vector<Car>& cars);
  void UpdateChannel(vector<Car>& cars, int channel, int direction);
void UpdateChannelLane(vector<Car>& cars,vector<vector<int>>& lane, int channel);
};

inline std::ostream& operator<<(std::ostream& ouput, Road road){
  // std<<cout
  for(int i=0; i<road.channel; i++){
    for(int j = 0; j< road.length; j++)
    {
      std::cout<<std::setw(6)<<road.lane0[i][j];
    }
    std::cout<<std::endl;
  }
  std::cout<<std::endl;
  for(int i=0; i<road.channel; i++){
    for(int j = 0; j< road.length; j++)
    {
      std::cout<<std::setw(6)<<road.lane1[i][j];
    }
    std::cout<<std::endl;
  }
    std::cout<<std::endl;  std::cout<<std::endl;
  return ouput;
}
#endif  // ROAD_H_
