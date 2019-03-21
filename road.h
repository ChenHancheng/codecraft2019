#ifndef ROAD_H_
#define ROAD_H_

#include <algorithm>
#include <queue>
#include <vector>

#include "car.h"
#include "codecraft_util.h"
#include "data_type.h"

#define POS_BLANK -1
#define POS_OCCUPATION 1

#define ADD_SUCCESS 0
#define FRONT_CAR_WAIT 1
#define NEXT_ROAD_FULL 2
using std::priority_queue;
using std::vector;
using std::min;

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
  // priority_queue<int> lane0_wait;
  vector<int> lane0_last_car;
  vector<int> lane0_last_time;

  vector<vector<int>> lane1;
  // priority_queue<int> lane1_wait;
  vector<int> lane1_last_car;
  vector<int> lane1_last_time;
  int cars_num_road;
// int s;
  void RoadRun(vector<Car>& cars);
  void RoadRunLane(vector<Car>& cars, vector<vector<int>>& lane);

  int QueryRoadStateLane(vector<Car>& cars, int car_id,
                         vector<vector<int>>& lane, vector<int>& lane_last_pos,
                         vector<int>& lane_last_time);
  int QueryRoadState(vector<Car>& cars, int car_id, int direction);

  int AddCarLane(vector<Car>& cars, int car_id, vector<vector<int>>& lane,
                  vector<int>& lane_last_pos, vector<int>& lane_last_time);
  int AddCar(vector<Car>& cars, int car_id, int direction);

  void UpdateCar(vector<Car>& cars, const int car_id);
  void UpdateCarLane(vector<Car>& cars, const int car_id, vector<vector<int>>& lane,
                     vector<int>& lane_last_pos, vector<int>& lane_last_time);

void DeleteCar(vector<Car>& cars, int car_id);
  // void DeleteCar(vector<Car>& cars, int car_id);
  // void DeleteCarLane(vector<Car>& cars, int car_id, vector<vector<int>>& lane,
  //                    vector<int>& lane_last_pos, vector<int>& lane_last_time);

  void UpdateLastCar(const vector<Car>& cars){
    for(int i=0; i<lane0.size(); i++){
      int j=0;
      for(j=0; j<length; j++){
        if(lane0[i][length-j-1] != POS_BLANK){
          lane0_last_car[i] = lane0[i][length-j-1];
          int car_id = lane0[i][length-j-1];
          lane0_last_time[i] = cars[car_id].pos/cars[car_id].speed;
          break;
        }
      }
      if(j == length){
        lane0_last_car[i] = POS_BLANK;
        lane0_last_time[i] = 0;
      }
    }
    for(int i=0; i<lane1.size(); i++){
      int j;
      for(j=0; j<length; j++){
        if(lane1[i][length-1-j] != POS_BLANK){
          lane1_last_car[i] = lane1[i][length-j-1];
          int car_id = lane1[i][length-j-1];
          lane1_last_time[i] = cars[car_id].pos/cars[car_id].speed;
          break;
        }
      }
      if(j == length){
        lane1_last_car[i] = POS_BLANK;
        lane1_last_time[i] = 0;
      }
    }
  }
};
#endif  // ROAD_H_
