#ifndef ROAD_H_
#define ROAD_H_

#include <vector>
#include <algorithm>

#include "codecraft_util.h"

using std::vector;

class Road{
  int id;
  int length;
  int speed_limit;
  int channel;
  int start;
  int end;
  bool bidirectional;

  vector<vector<int>> lane_end;
  vector<vector<int>> lane_start;

  
  int cars_num_road;
  void RoadRun(vector<Car>& cars);
  void RoadRunLane(vector<Car>& cars, vector<vector<int>>& lane)
};

void Road::RoadRun(vector<Car>& cars){
  RoadRunLane(cars, lane_end);
  if(bidirectional){
    RoadRunLane(cars, lane_start);
  }
}
void Road::RoadRunLane(vector<Car>& cars, vector<vector<int>>& lane){
  for(int i=0; i<channel; i++){
    for(int j=0; j<length; j++){
      int car_id = cars_end[i][j];
      int last_car_id = 0;
      if(car_id != 0){
        if(last_car_id == 0){
            if(cars[car_id].speed+j<=length){
              cars[car_id].state = STOP;
              cars[car_id].pos = cars[car_id].speed+j;

              lane[i][j] = 0;
              lane[i][cars[car_id].pos] = cars[car_id].pos;
            else{
              cars[car_id].state = STOP;
            }
        }
        else {
          if(cars[last_car_id].state == WAIT){
            if(cars[car_id].speed+j< cars[last_car_id].pos){
              cars[car_id].pos = cars[car_id].speed+j;
              cars[car_id].state = STOP;

              lane[i][j] = 0;
              lane[i][cars[car_id].pos] = cars[car_id].pos;
            }
            else{
              cars[car_id].state = WAIT;
            }
          }

          if(cars[last_car_id].state == STOP){
            cars[car_id].pos = min(cars[car_id].speed+j, cars[last_car_id].pos-1);
            cars[car_id].state = STOP;
          }
        }
        last_car_id = car_id;
      }
    }
  }
}

#endif //ROAD_H_
